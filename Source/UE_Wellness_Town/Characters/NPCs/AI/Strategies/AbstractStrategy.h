// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AbstractStrategy.generated.h"

class UGOAP_Agent;

UCLASS(Abstract)
class UE_WELLNESS_TOWN_API UAbstractStrategy : public UObject
{
	GENERATED_BODY()
	
public:
	virtual bool CanPerform();
	virtual bool Complete();

	virtual void Start(UGOAP_Agent* agent);
	virtual void Update(float deltaTime);
	virtual void Stop();
};
