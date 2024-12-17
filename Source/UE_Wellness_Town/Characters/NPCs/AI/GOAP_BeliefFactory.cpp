// Fill out your copyright notice in the Description page of Project Settings.


#include "GOAP_BeliefFactory.h"
#include "UE_Wellness_Town/Characters/NPCs/AI/GOAP_Belief.h"

UGOAP_BeliefFactory::UGOAP_BeliefFactory()
{
}

void UGOAP_BeliefFactory::Init(TMap<FString, TObjectPtr<UGOAP_Belief>>* beliefs)
{
	_beliefs = beliefs;
}

void UGOAP_BeliefFactory::AddBelief(FString name, TFunction<bool()> condition)
{
	_beliefs->Add(name, UGOAP_Belief::Builder(name).WithCondition(condition).Build());
}

void UGOAP_BeliefFactory::AddLocationBelief(FString name, FVector location)
{
	_beliefs->Add(name, UGOAP_Belief::Builder(name).WithLocation(location).Build());
}
