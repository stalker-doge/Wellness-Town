// Fill out your copyright notice in the Description page of Project Settings.


#include "SleepStrategy.h"
#include "UE_Wellness_Town/Characters/NPCs/AI/GOAP_Agent.h"
#include "UE_Wellness_Town/Characters/NPCs/NPC_Base.h"

void USleepStrategy::Start(UGOAP_Agent* agent)
{
	_agent = agent;
	_agent->_owner->SetActorRotation(_agent->_owner->GetActorRotation() + FRotator(0, 0, 90));
}

void USleepStrategy::Update(float deltaTime)
{
}

void USleepStrategy::Stop()
{
	_agent->_owner->SetActorRotation(_agent->_owner->GetActorRotation() + FRotator(0, 0, -90));
}

bool USleepStrategy::Complete()
{
	return !_agent->_isSleepHours;
}
