// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UE_Wellness_Town/Characters/NPCs/AI/Strategies/AbstractStrategy.h"
#include "TravelToHomeStrategy.generated.h"

class UGOAP_Agent;

UCLASS()
class UE_WELLNESS_TOWN_API UTravelToHomeStrategy : public UAbstractStrategy
{
	GENERATED_BODY()
	
public:

	virtual void Start(UGOAP_Agent* agent) override;
	virtual void Update(float deltaTime) override;
	virtual void Stop() override;

	virtual bool Complete() override;

private:
	bool IsAtHome();

private:
	TWeakObjectPtr<UGOAP_Agent> _agent;
	TWeakObjectPtr<AActor> _home;

	FVector _target;
};
