// Fill out your copyright notice in the Description page of Project Settings.


#include "GOAP_Agent.h"
#include "UE_Wellness_Town/GameModes/WT_GameMode.h"
#include "UE_Wellness_Town/Characters/NPCs/NPC_Base.h"
#include "UE_Wellness_Town/Characters/NPCs/AI/GOAP_Belief.h"
#include "UE_Wellness_Town/Characters/NPCs/AI/GOAP_Goal.h"
#include "UE_Wellness_Town/Characters/NPCs/AI/GOAP_Action.h"
#include "UE_Wellness_Town/Characters/NPCs/AI/GOAP_Node.h"
#include "UE_Wellness_Town/Characters/NPCs/AI/GOAP_Planner.h"
#include "UE_Wellness_Town/Characters/NPCs/AI/GOAP_Plan.h"
#include "UE_Wellness_Town/Characters/NPCs/AI/GOAP_BeliefFactory.h"
#include "UE_Wellness_Town/Characters/NPCs/AI/Strategies/WanderStrategy.h"
#include "UE_Wellness_Town/Characters/NPCs/AI/Strategies/TalkStrategy.h"
#include "UE_Wellness_Town/Characters/NPCs/AI/Strategies/SleepStrategy.h"
#include "UE_Wellness_Town/Characters/NPCs/AI/Strategies/TravelToWorkStrategy.h"
#include "UE_Wellness_Town/Characters/NPCs/AI/Strategies/TravelToHomeStrategy.h"
#include "UE_Wellness_Town/Characters/NPCs/AI/Strategies/FishingWorkStrategy.h"
#include "UE_Wellness_Town/Characters/NPCs/AI/Strategies/BugCatcherWorkStrategy.h"
#include "UE_Wellness_Town/Characters/NPCs/AI/Strategies/MarketWorkStrategy.h"
#include "UE_Wellness_Town/Characters/NPCs/AI/WorkType.h"

// Sets default values for this component's properties
UGOAP_Agent::UGOAP_Agent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UGOAP_Agent::Init()
{
	_owner = Cast<ANPC_Base>(GetOwner());
}

#pragma region "GOAP"
void UGOAP_Agent::SetupBeliefs()
{
	UGOAP_BeliefFactory* factory = NewObject<UGOAP_BeliefFactory>();
	factory->Init(&_beliefs);

	factory->AddBelief(TEXT("WANDER"), [&] { return false; });

	factory->AddBelief(TEXT("WORKING"), [&] { return _isAtWorkPlace; });
	factory->AddBelief(TEXT("IS_AT_WORKPLACE"), [&] { return _isAtWorkPlace; });
	factory->AddBelief(TEXT("NEED_TO_WORK"), [&] { return _isWorkHours; });

	factory->AddBelief(TEXT("SLEEPING"), [&] { return _isAtHome; });
	factory->AddBelief(TEXT("NEED_TO_SLEEP"), [&] { return _isSleepHours; });

	factory->AddBelief(TEXT("IS_AT_HOME"), [&] { return _isAtHome; });
}

void UGOAP_Agent::SetupActions()
{
	_actions.Add(UGOAP_Action::Builder(TEXT("WANDER")).WithStrategy(NewObject<UWanderStrategy>()).AddEffect(_beliefs.FindChecked(TEXT("WANDER"))).Build());

	switch (_workType)
	{
	case WorkType::Fishing:
		_actions.Add(UGOAP_Action::Builder(TEXT("WORK")).WithStrategy(NewObject<UFishingWorkStrategy>()).AddPrecondition(_beliefs.FindChecked(TEXT("IS_AT_WORKPLACE"))).AddEffect(_beliefs.FindChecked(TEXT("WORKING"))).Build());
		break;
	case WorkType::Trading:
		// Trader work not implemented, will just stand at stall during work hours.
		_actions.Add(UGOAP_Action::Builder(TEXT("WORK")).WithStrategy(NewObject<UMarketWorkStrategy>()).AddPrecondition(_beliefs.FindChecked(TEXT("IS_AT_WORKPLACE"))).AddEffect(_beliefs.FindChecked(TEXT("WORKING"))).Build());
		break;
	case WorkType::BugCatching:
		_actions.Add(UGOAP_Action::Builder(TEXT("WORK")).WithStrategy(NewObject<UBugCatcherWorkStrategy>()).AddPrecondition(_beliefs.FindChecked(TEXT("IS_AT_WORKPLACE"))).AddEffect(_beliefs.FindChecked(TEXT("WORKING"))).Build());
		break;
	default:
		break;
	}

	_actions.Add(UGOAP_Action::Builder(TEXT("TRAVEL_TO_WORK")).WithStrategy(NewObject<UTravelToWorkStrategy>()).AddPrecondition(_beliefs.FindChecked(TEXT("NEED_TO_WORK"))).AddEffect(_beliefs.FindChecked(TEXT("IS_AT_WORKPLACE"))).Build());

	_actions.Add(UGOAP_Action::Builder(TEXT("SLEEP")).WithStrategy(NewObject<USleepStrategy>()).AddPrecondition(_beliefs.FindChecked(TEXT("IS_AT_HOME"))).AddEffect(_beliefs.FindChecked(TEXT("SLEEPING"))).Build());
	_actions.Add(UGOAP_Action::Builder(TEXT("TRAVEL_TO_HOME")).WithStrategy(NewObject<UTravelToHomeStrategy>()).AddPrecondition(_beliefs.FindChecked(TEXT("NEED_TO_SLEEP"))).AddEffect(_beliefs.FindChecked(TEXT("IS_AT_HOME"))).Build());
}

void UGOAP_Agent::SetupGoals()
{
	_goals.Add(UGOAP_Goal::Builder(TEXT("WANDER")).WithPriority(1).AddDesiredEffect(_beliefs.FindChecked(TEXT("WANDER"))).Build());
	_goals.Add(UGOAP_Goal::Builder(TEXT("WORK")).WithPriority(3).AddDesiredEffect(_beliefs.FindChecked(TEXT("WORKING"))).Build());
	_goals.Add(UGOAP_Goal::Builder(TEXT("SLEEP")).WithPriority(5).AddDesiredEffect(_beliefs.FindChecked(TEXT("SLEEPING"))).Build());
}

void UGOAP_Agent::CalculateActionPlan()
{
	int priorityLevel = (_currentGoal != nullptr) ? _currentGoal->_priority : 0;

	TArray<UGOAP_Goal*> goalsToCheck;

	if (_currentGoal != nullptr)
	{
		goalsToCheck = _goals.FilterByPredicate([&](const UGOAP_Goal* index) { return index->_priority > priorityLevel; });
	}
	else
	{
		goalsToCheck = _goals;
	}

	UGOAP_Plan* potentialPlan = _planner->Plan(this, _goals, nullptr);

	if (potentialPlan != nullptr)
	{
		_plan = potentialPlan;
	}
}

void UGOAP_Agent::Reset()
{
	_goals.Empty();
	_beliefs.Empty();
	_actions.Empty();

	_planner = nullptr;

	SetupBeliefs();
	SetupActions();
	SetupGoals();

	_planner = NewObject<UGOAP_Planner>();
}

TMap<FString, TObjectPtr<UGOAP_Belief>> UGOAP_Agent::GetBeliefs() const
{
	return _beliefs;
}

TArray<TObjectPtr<UGOAP_Action>> UGOAP_Agent::GetActions() const
{
	return _actions;
}

TArray<TObjectPtr<UGOAP_Goal>> UGOAP_Agent::GetGoals() const
{
	return _goals;
}
#pragma endregion

#pragma region "AI"
void UGOAP_Agent::SetPauseAgent(bool val)
{
	if (_isPaused == val)
	{
		return;
	}

	_isPaused = val;

	_owner->SetPauseMovement(_isPaused);
}

void UGOAP_Agent::TogglePauseAgent()
{
	SetPauseAgent(!_isPaused);
}

void UGOAP_Agent::SetDestination(FVector destination)
{
	_owner->SetDestination(destination);
}

AActor* UGOAP_Agent::GetHome() const
{
	return _home;
}

AActor* UGOAP_Agent::GetWorkPlace() const
{
	return _workPlace;
}

WorkType UGOAP_Agent::GetWorkType() const
{
	return _workType;
}

void UGOAP_Agent::SetIsAtHome(bool val)
{
	_isAtHome = val;
}

void UGOAP_Agent::SetIsAtWorkPlace(bool val)
{
	_isAtWorkPlace = val;
}
bool UGOAP_Agent::IsAtHome() const
{
	return _isAtHome;
}
bool UGOAP_Agent::IsAtWorkPlace() const
{
	return _isAtWorkPlace;
}
bool UGOAP_Agent::IsSleepHours() const
{
	return _isSleepHours;
}
bool UGOAP_Agent::IsWorkHours() const
{
	return _isWorkHours;
}
#pragma endregion

#pragma region "Helpers"
USkeletalMeshComponent* UGOAP_Agent::GetMesh()
{
	return _owner->GetMesh();
}

ANPC_Base* UGOAP_Agent::GetNPC()
{
	return _owner.Get();
}

FVector UGOAP_Agent::GetCurrentDestination() const
{
	return _owner->GetCurrentDestination();
}

FVector UGOAP_Agent::GetActorLocation() const
{
	return _owner->GetActorLocation();
}

FVector UGOAP_Agent::GetForwardVector() const
{
	return _owner->GetActorForwardVector();;
}

TSubclassOf<AFishingRod> UGOAP_Agent::GetFishingRodDefault() const
{
	return _fishingRodDefault;
}

TSubclassOf<ABugNet> UGOAP_Agent::GetBugNetDefault() const
{
	return _bugNetDefault;
}

bool UGOAP_Agent::HasPath() const
{
	return _owner->HasPath();
}
#pragma endregion

// Called when the game starts
void UGOAP_Agent::BeginPlay()
{
	Super::BeginPlay();

	SetupBeliefs();
	SetupActions();
	SetupGoals();

	_planner = NewObject<UGOAP_Planner>();
}


// Called every frame
void UGOAP_Agent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	_timer += DeltaTime * 0.25f;

	if (_timer >= 24)
	{
		_timer = 0;
	}

	if (_isPaused == true)
	{
		return;
	}

	_isWorkHours = (_timer >= 9 && _timer < 17);
	_isSleepHours = (_timer >= 22 || _timer < 6);

	if (_currentAction == nullptr)
	{
		CalculateActionPlan();

		if (_plan != nullptr && _plan->actions.Num() > 0)
		{
			_currentGoal = _plan->goal;
			_currentAction = _plan->actions.Pop();

			_currentAction->Start(this);
		}
	}

	if (_plan != nullptr && _currentAction != nullptr)
	{
		_currentAction->Update(DeltaTime);

		if (_currentAction->IsComplete() == true)
		{
			_currentAction->Stop();
			_currentAction = nullptr;

			if (_plan->actions.Num() == 0)
			{
				_lastGoal = _currentGoal;
				_currentGoal = nullptr;
			}
			else
			{
				_currentAction = _plan->actions.Pop();
				_currentAction->Start(this);
			}
		}
	}

	if (_currentAction == nullptr)
	{
		_planFailCounter += 1;

		if (_planFailCounter > 5)
		{
			Reset();
		}
	}
	else
	{
		_planFailCounter = 0;
	}
}

