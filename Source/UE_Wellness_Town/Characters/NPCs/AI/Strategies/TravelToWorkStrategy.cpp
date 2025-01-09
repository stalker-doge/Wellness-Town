// Fill out your copyright notice in the Description page of Project Settings.


#include "TravelToWorkStrategy.h"
#include "UE_Wellness_Town/Characters/NPCs/AI/GOAP_Agent.h"

void UTravelToWorkStrategy::Start(UGOAP_Agent* agent)
{
	_agent = agent;
	_agent->SetIsAtWorkPlace(false);

	_workPlace = _agent->GetWorkPlace();

	if (IsAtWorkPlace() == false)
	{
		_target = _workPlace->GetActorLocation();
		_agent->SetDestination(_target);
	}
}

void UTravelToWorkStrategy::Update(float deltaTime)
{
	if (_agent->GetCurrentDestination() != _target)
	{
		_agent->SetDestination(_target);
	}

	if (IsAtWorkPlace() == false)
	{
		return;
	}

	_agent->SetIsAtWorkPlace(true);
}

void UTravelToWorkStrategy::Stop()
{
}

bool UTravelToWorkStrategy::Complete()
{
    return _agent->IsAtWorkPlace();
}

bool UTravelToWorkStrategy::IsAtWorkPlace()
{
	if (_workPlace == nullptr)
	{
		return false;
	}

	FVector distance = _agent->GetActorLocation() - _workPlace->GetActorLocation();

	if (distance.Length() > 100)
	{
		return false;
	}

	return true;
}
