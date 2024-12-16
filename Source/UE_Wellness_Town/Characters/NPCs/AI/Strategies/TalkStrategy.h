// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UE_Wellness_Town/Interfaces/ActionStrategy.h"
#include "UE_Wellness_Town/Characters/NPCs/AI/Strategies/AbstractStrategy.h"
#include "TalkStrategy.generated.h"

class UGOAP_Agent;

UCLASS()
class UE_WELLNESS_TOWN_API UTalkStrategy : public UAbstractStrategy
{
	GENERATED_BODY()

public:
	virtual void Start(UGOAP_Agent* agent) override;
	virtual void Update(float deltaTime) override;
	virtual void Stop() override;

	virtual bool Complete() override;

private:
	TObjectPtr<UGOAP_Agent> _agent;
	TObjectPtr<UGOAP_Agent> _dialogueTarget;

	float _timer;
};
