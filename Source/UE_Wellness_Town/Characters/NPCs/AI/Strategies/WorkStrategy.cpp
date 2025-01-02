// Fill out your copyright notice in the Description page of Project Settings.


#include "WorkStrategy.h"
#include "UE_Wellness_Town/Characters/NPCs/AI/GOAP_Agent.h"

void UWorkStrategy::Start(UGOAP_Agent* agent)
{
	_agent = agent;
	_workPlace = _agent->GetWorkPlace();
}

void UWorkStrategy::Update(float deltaTime)
{
	GEngine->AddOnScreenDebugMessage(20, 1, FColor::Blue, FString::Printf(TEXT("Working")));
}

void UWorkStrategy::Stop()
{
	_agent->SetIsAtWorkPlace(false);
}

bool UWorkStrategy::Complete()
{
	return !_agent->_isWorkHours;
}
