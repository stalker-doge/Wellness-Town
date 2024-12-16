// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ActionStrategy.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UActionStrategy : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UE_WELLNESS_TOWN_API IActionStrategy
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual bool CanPerform();
	virtual bool Complete();

	virtual void Start();
	virtual void Update(float deltaTime);
	virtual void Stop();
};
