// Fill out your copyright notice in the Description page of Project Settings.


#include "WanderStrategy.h"
#include "NavigationSystem.h"
#include "UE_Wellness_Town/Characters/NPCs/NPC_Base.h"
#include "UE_Wellness_Town/Characters/NPCs/AI/GOAP_Agent.h"

void UWanderStrategy::Start(UGOAP_Agent* agent)
{
    _agent = agent;
    _navSystem = UNavigationSystemV1::GetNavigationSystem(_agent->_owner->GetWorld());
    _timer = 0;
    _delayBetweenPathing = 1;
}

void UWanderStrategy::Update(float deltaTime)
{
	if (_agent->HasPath() == true)
	{
		return;
	}

	_timer += deltaTime;

	if (_timer >= _delayBetweenPathing)
	{
		FNavLocation location;
		_navSystem->GetRandomPointInNavigableRadius(_agent->_owner->GetActorLocation(), 500, location);

		_agent->SetDestination(location);
		_timer = 0;
	}
}

void UWanderStrategy::Stop()
{
	_timer = 0;
	_agent->SetDestination(_agent->_owner->GetActorLocation());
}

bool UWanderStrategy::Complete()
{
    return _agent->_isWorkHours || _agent->_isSleepHours;
}
