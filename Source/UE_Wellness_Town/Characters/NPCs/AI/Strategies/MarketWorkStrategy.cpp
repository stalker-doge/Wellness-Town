// Fill out your copyright notice in the Description page of Project Settings.


#include "MarketWorkStrategy.h"
#include "UE_Wellness_Town/Characters/NPCs/AI/GOAP_Agent.h"

void UMarketWorkStrategy::Start(UGOAP_Agent* agent)
{
	_agent = agent;
	_workPlace = _agent->GetWorkPlace();
}

void UMarketWorkStrategy::Update(float deltaTime)
{
	GEngine->AddOnScreenDebugMessage(20, 1, FColor::Blue, FString::Printf(TEXT("Working")));
}

void UMarketWorkStrategy::Stop()
{
	_agent->SetIsAtWorkPlace(false);
}

bool UMarketWorkStrategy::Complete()
{
	return !_agent->IsWorkHours();
}