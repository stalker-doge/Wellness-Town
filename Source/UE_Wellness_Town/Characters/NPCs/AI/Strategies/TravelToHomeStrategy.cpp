// Fill out your copyright notice in the Description page of Project Settings.


#include "TravelToHomeStrategy.h"
#include "UE_Wellness_Town/Characters/NPCs/AI/GOAP_Agent.h"

void UTravelToHomeStrategy::Start(UGOAP_Agent* agent)
{
	_agent = agent;
	_agent->SetIsAtHome(false);

	_home = _agent->GetHome();

	if (IsAtHome() == false)
	{
		_target = _home->GetActorLocation();
		_agent->SetDestination(_target);
	}
}

void UTravelToHomeStrategy::Update(float deltaTime)
{
	if (_agent->GetCurrentDestination() != _target)
	{
		_agent->SetDestination(_target);
	}

	if (IsAtHome() == false)
	{
		return;
	}

	_agent->SetIsAtHome(true);
}

void UTravelToHomeStrategy::Stop()
{
}

bool UTravelToHomeStrategy::Complete()
{
	return _agent->IsAtHome();
}

bool UTravelToHomeStrategy::IsAtHome()
{
	if (_home == nullptr)
	{
		return false;
	}

	FVector distance = _agent->GetActorLocation() - _home->GetActorLocation();

	if (distance.Length() > 100)
	{
		return false;
	}

	return true;
}
