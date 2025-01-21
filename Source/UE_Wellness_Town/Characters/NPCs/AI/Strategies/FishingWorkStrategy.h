// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UE_Wellness_Town/Characters/NPCs/AI/Strategies/AbstractStrategy.h"
#include "FishingWorkStrategy.generated.h"

class UGOAP_Agent;
class ANPC_Base;
class AFishingRod;
class USplineComponent;

UCLASS()
class UE_WELLNESS_TOWN_API UFishingWorkStrategy : public UAbstractStrategy
{
	GENERATED_BODY()
	
public:
	virtual void Start(UGOAP_Agent* agent) override;
	virtual void Update(float deltaTime) override;
	virtual void Stop() override;

	virtual bool Complete() override;

private:
	void CreateSpline();
	void CastRod();

private:
	TWeakObjectPtr<UGOAP_Agent> _agent;
	TWeakObjectPtr<ANPC_Base> _npc;

	UPROPERTY()
	TObjectPtr<AFishingRod> _fishingRod;

	UPROPERTY()
	TObjectPtr<USplineComponent> _splineComponent;

	float _actionTimer;
	float _actionDelay;
};
