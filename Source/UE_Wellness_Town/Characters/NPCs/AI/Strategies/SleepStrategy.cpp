// Fill out your copyright notice in the Description page of Project Settings.


#include "SleepStrategy.h"
#include "UE_Wellness_Town/Characters/NPCs/AI/GOAP_Agent.h"
#include "UE_Wellness_Town/Characters/NPCs/NPC_Base.h"

void USleepStrategy::Start(UGOAP_Agent* agent)
{
	_agent = agent;
	ToggleSleep(true);
}

void USleepStrategy::Update(float deltaTime)
{
	GEngine->AddOnScreenDebugMessage(20, 1, FColor::Blue, FString::Printf(TEXT("Sleeping")));
}

void USleepStrategy::Stop()
{
	_agent->SetIsAtHome(false);
	ToggleSleep(false);
}

bool USleepStrategy::Complete()
{
	return !_agent->_isSleepHours;
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
