// Fill out your copyright notice in the Description page of Project Settings.


#include "WorkStrategy.h"
#include "UE_Wellness_Town/Characters/NPCs/AI/GOAP_Agent.h"

void UWorkStrategy::Start(UGOAP_Agent* agent)
{
	_agent = agent;
}

void UWorkStrategy::Update(float deltaTime)
{
}

void UWorkStrategy::Stop()
{
}

bool UWorkStrategy::Complete()
{
	return !_agent->_isWorkHours;
}
