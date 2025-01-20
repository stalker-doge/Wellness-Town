// Fill out your copyright notice in the Description page of Project Settings.


#include "WanderStrategy.h"
#include "NavigationSystem.h"
#include "UE_Wellness_Town/Characters/NPCs/NPC_Base.h"
#include "UE_Wellness_Town/Characters/NPCs/AI/GOAP_Agent.h"

void UWanderStrategy::Start(UGOAP_Agent* agent)
{
    _agent = agent;
    _navSystem = UNavigationSystemV1::GetNavigationSystem(_agent->GetNPC()->GetWorld());
    _timer = 0;
    _delayBetweenPathing = 1;
}

void UWanderStrategy::Update(float deltaTime)
{
	GEngine->AddOnScreenDebugMessage(20, 1, FColor::Blue, FString::Printf(TEXT("Wandering")));

	if (_agent->HasPath() == true)
	{
		return;
	}

	_timer += deltaTime;

	if (_timer >= _delayBetweenPathing)
	{
		FNavLocation location;
		_navSystem->GetRandomPointInNavigableRadius(_agent->GetNPC()->GetActorLocation(), 500, location);

		_agent->SetDestination(location);
		_timer = 0;
	}
}

void UWanderStrategy::Stop()
{
	_timer = 0;
	_agent->SetDestination(_agent->GetNPC()->GetActorLocation());
}

bool UWanderStrategy::Complete()
{
    return _agent->IsWorkHours() || _agent->IsSleepHours();
}
