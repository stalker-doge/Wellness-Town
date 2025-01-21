// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UE_Wellness_Town/Characters/NPCs/AI/Strategies/AbstractStrategy.h"
#include "BugCatcherWorkStrategy.generated.h"

class UGOAP_Agent;
class UGOAP_BugSensor;
class ANPC_Base;
class ABug;
class ABugNet;

UCLASS()
class UE_WELLNESS_TOWN_API UBugCatcherWorkStrategy : public UAbstractStrategy
{
	GENERATED_BODY()
	
public:
	void CreateSensor();
	bool IsInTargetRange();

	virtual void Start(UGOAP_Agent* agent) override;
	virtual void Update(float deltaTime) override;
	virtual void Stop() override;

	virtual bool Complete() override;
private:
	TWeakObjectPtr<UGOAP_Agent> _agent;
	TWeakObjectPtr<ANPC_Base> _npc;
	TWeakObjectPtr<ABug> _target;

	UPROPERTY()
	TObjectPtr<UGOAP_BugSensor> _sensor;

	UPROPERTY()
	TObjectPtr<ABugNet> _bugNet;

	float _timer;
};
