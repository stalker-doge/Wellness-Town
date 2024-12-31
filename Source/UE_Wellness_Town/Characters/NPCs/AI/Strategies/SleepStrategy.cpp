// Fill out your copyright notice in the Description page of Project Settings.


#include "SleepStrategy.h"
#include "UE_Wellness_Town/Characters/NPCs/AI/GOAP_Agent.h"
#include "UE_Wellness_Town/Characters/NPCs/NPC_Base.h"

void USleepStrategy::Start(UGOAP_Agent* agent)
{
	_isSleeping = false;

	_agent = agent;
	_home = _agent->GetHome();

	if (IsAtHome() == false)
	{
		_agent->SetDestination(_home->GetActorLocation());
	}
}

void USleepStrategy::Update(float deltaTime)
{
	if (IsAtHome() == false)
	{
		return;
	}

	if (_isSleeping == false)
	{
		_isSleeping = true;
		ToggleSleep(_isSleeping);
	}

	GEngine->AddOnScreenDebugMessage(21, 1, FColor::Blue, FString::Printf(TEXT("Sleeping")));
}

void USleepStrategy::Stop()
{
	if (_isSleeping == true)
	{
		ToggleSleep(false);
	}
}

bool USleepStrategy::Complete()
{
	return !_agent->_isSleepHours;
}

bool USleepStrategy::IsAtHome()
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

void USleepStrategy::ToggleSleep(bool isSleeping)
{
	_agent->GetMesh()->SetVisibility(!isSleeping);

	if (isSleeping == true)
	{
		_agent->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else
	{
		_agent->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}
