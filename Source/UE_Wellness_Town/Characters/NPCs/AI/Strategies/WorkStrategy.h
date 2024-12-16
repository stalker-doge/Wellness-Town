// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UE_Wellness_Town/Characters/NPCs/AI/Strategies/AbstractStrategy.h"
#include "WorkStrategy.generated.h"

/**
 * 
 */
UCLASS()
class UE_WELLNESS_TOWN_API UWorkStrategy : public UAbstractStrategy
{
	GENERATED_BODY()
	
public:
	virtual void Start(UGOAP_Agent* agent) override;
	virtual void Update(float deltaTime) override;
	virtual void Stop() override;

	virtual bool Complete() override;

private:
	TObjectPtr<UGOAP_Agent> _agent;
};