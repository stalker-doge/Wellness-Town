// Fill out your copyright notice in the Description page of Project Settings.


#include "WorkStrategy.h"
#include "UE_Wellness_Town/Characters/NPCs/AI/GOAP_Agent.h"

void UWorkStrategy::Start(UGOAP_Agent* agent)
{
	_agent = agent;
	_workPlace = _agent->GetWorkPlace();
	_workDirectionTarget = _agent->GetWorkDirectionTarget();

	_isWorking = false;

	if (IsAtWorkPlace() == false)
	{
		_agent->SetDestination(_workPlace->GetActorLocation());
	}
}

void UWorkStrategy::Update(float deltaTime)
{
	if (_isWorking == true)
	{
		GEngine->AddOnScreenDebugMessage(20, 1, FColor::Blue, FString::Printf(TEXT("Working")));
		return;
	}

	if (IsAtWorkPlace() == true)
	{
		_isWorking = true;

		if (_workDirectionTarget != nullptr)
		{
			_agent->SetActorLookAt(_workDirectionTarget);
		}
	}
}

void UWorkStrategy::Stop()
{
}

bool UWorkStrategy::Complete()
{
	return !_agent->_isWorkHours;
}

bool UWorkStrategy::IsAtWorkPlace()
{
	if (_workPlace == nullptr)
	{
		return false;
	}

	FVector distance = _agent->GetActorLocation() - _workPlace->GetActorLocation();

	if (distance.Length() > 20)
	{
		return false;
	}

	return true;
}
