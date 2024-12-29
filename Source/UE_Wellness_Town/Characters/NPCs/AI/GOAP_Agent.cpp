// Fill out your copyright notice in the Description page of Project Settings.


#include "GOAP_Agent.h"
#include "UE_Wellness_Town/GameModes/WT_GameMode.h"
#include "UE_Wellness_Town/Utility/TimeManager.h"
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
#include "UE_Wellness_Town/Characters/NPCs/AI/Strategies/WorkStrategy.h"
#include "UE_Wellness_Town/Characters/NPCs/AI/Strategies/SleepStrategy.h"
//#include "UE_Wellness_Town/Characters/NPCs/AI/Sensors/GOAP_NPCSensor.h"
//#include "Components/SphereComponent.h"

// Sets default values for this component's properties
UGOAP_Agent::UGOAP_Agent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//_npcSensor = CreateDefaultSubobject<UGOAP_NPCSensor>(TEXT("NPC Sensor"));
	//checkf(_npcSensor, TEXT("GOAP Agent missing NPC Sensor"));
}

void UGOAP_Agent::Init()
{
	_owner = Cast<ANPC_Base>(GetOwner());
	checkf(_owner, TEXT("GOAP_Agent Owner Not Found"));

	//_npcSensor->_sensor->SetupAttachment(_owner->GetRootComponent());
}

void UGOAP_Agent::SetupBeliefs()
{
	UGOAP_BeliefFactory* factory = NewObject<UGOAP_BeliefFactory>();
	factory->Init(&_beliefs);

	factory->AddBelief(TEXT("WANDER"), [&] { return false; });
	factory->AddBelief(TEXT("NEED_TO_WORK"), [&] { return !_isWorkHours; });
	factory->AddBelief(TEXT("NEED_TO_SLEEP"), [&] { return !_isSleepHours; });
}

void UGOAP_Agent::SetupActions()
{
	_actions.Add(UGOAP_Action::Builder(TEXT("WANDER")).WithStrategy(NewObject<UWanderStrategy>()).AddEffect(_beliefs.FindChecked(TEXT("WANDER"))).Build());
	_actions.Add(UGOAP_Action::Builder(TEXT("WORK")).WithStrategy(NewObject<UWorkStrategy>()).AddEffect(_beliefs.FindChecked(TEXT("NEED_TO_WORK"))).Build());
	_actions.Add(UGOAP_Action::Builder(TEXT("SLEEP")).WithStrategy(NewObject<USleepStrategy>()).AddEffect(_beliefs.FindChecked(TEXT("NEED_TO_SLEEP"))).Build());
}

void UGOAP_Agent::SetupGoals()
{
	_goals.Add(UGOAP_Goal::Builder(TEXT("WANDER")).WithPriority(1).AddDesiredEffect(_beliefs.FindChecked(TEXT("WANDER"))).Build());
	_goals.Add(UGOAP_Goal::Builder(TEXT("WORK")).WithPriority(30).AddDesiredEffect(_beliefs.FindChecked(TEXT("NEED_TO_WORK"))).Build());
	_goals.Add(UGOAP_Goal::Builder(TEXT("SLEEP")).WithPriority(5).AddDesiredEffect(_beliefs.FindChecked(TEXT("NEED_TO_SLEEP"))).Build());
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

void UGOAP_Agent::SetDestination(FVector destination)
{
	_owner->SetDestination(destination);
}

FVector UGOAP_Agent::GetActorLocation()
{
	return _owner->GetActorLocation();
}

FVector UGOAP_Agent::GetForwardVector()
{
	return _owner->GetActorForwardVector();;
}

bool UGOAP_Agent::HasPath()
{
	return _owner->HasPath();
}


// Called when the game starts
void UGOAP_Agent::BeginPlay()
{
	Super::BeginPlay();

	_timeManager = Cast<AWT_GameMode>(GetWorld()->GetAuthGameMode())->GetTimeManager();
	checkf(_timeManager, TEXT("No Time Manager"));

	//_npcSensor->Init(150, this);

	SetupBeliefs();
	SetupActions();
	SetupGoals();

	_planner = NewObject<UGOAP_Planner>();
	_canTalk = true;
}


// Called every frame
void UGOAP_Agent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	_isWorkHours = (_timeManager->GetHours() >= 9 && _timeManager->GetHours() < 17);
	_isSleepHours = (_timeManager->GetHours() >= 22 || _timeManager->GetHours() < 6);

	//GEngine->AddOnScreenDebugMessage(11, 5, FColor::Red, FString::Printf(TEXT("Work Hours: %s"), _isWorkHours ? TEXT("TRUE") : TEXT("FALSE")));
	//GEngine->AddOnScreenDebugMessage(12, 5, FColor::Red, FString::Printf(TEXT("Sleep Hours: %s"), _isSleepHours ? TEXT("TRUE") : TEXT("FALSE")));

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
			}
		}
	}

	if (_currentAction == nullptr)
	{
		_planFailCounter += 1;

		GEngine->AddOnScreenDebugMessage(10, 5, FColor::Red, FString::Printf(TEXT("Counter: %i"), _planFailCounter));

		if (_planFailCounter > 5)
		{
			Reset();
		}

		GEngine->AddOnScreenDebugMessage(11, 5, FColor::Red, FString::Printf(TEXT("Current Action: NONE")));
	}
	else
	{
		_planFailCounter = 0;
		GEngine->AddOnScreenDebugMessage(12, 5, FColor::Red, FString::Printf(TEXT("Current Action: %s"), *_currentAction->_name));
	}
}

