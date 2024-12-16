// Fill out your copyright notice in the Description page of Project Settings.


#include "GOAP_Agent.h"
#include "UE_Wellness_Town/Characters/NPCs/NPC_Base.h"
#include "UE_Wellness_Town/Characters/NPCs/AI/GOAP_Belief.h"
#include "UE_Wellness_Town/Characters/NPCs/AI/GOAP_Goal.h"
#include "UE_Wellness_Town/Characters/NPCs/AI/GOAP_Action.h"
#include "UE_Wellness_Town/Characters/NPCs/AI/GOAP_Node.h"
#include "UE_Wellness_Town/Characters/NPCs/AI/GOAP_Planner.h"
#include "UE_Wellness_Town/Characters/NPCs/AI/GOAP_Plan.h"
#include "UE_Wellness_Town/Characters/NPCs/AI/GOAP_BeliefFactory.h"

// Sets default values for this component's properties
UGOAP_Agent::UGOAP_Agent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UGOAP_Agent::Init()
{
	_owner = Cast<ANPC_Base>(GetOwner());
	checkf(_owner, TEXT("GOAP_Agent Owner Not Found"));
}

void UGOAP_Agent::SetupBeliefs()
{
	UGOAP_BeliefFactory* factory = NewObject<UGOAP_BeliefFactory>();
	factory->Init(&_beliefs);

	factory->AddBelief(TEXT("WANDER"), [&] { return true; });
}

void UGOAP_Agent::SetupActions()
{
	_actions.Add(UGOAP_Action::Builder(TEXT("WANDER")).AddEffect(_beliefs.FindChecked(TEXT("WANDER"))).Build());
}

void UGOAP_Agent::SetupGoals()
{
	_goals.Add(UGOAP_Goal::Builder(TEXT("WANDER")).WithPriority(1).AddDesiredEffect(_beliefs.FindChecked(TEXT("WANDER"))).Build());
}


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

	// ...
}

