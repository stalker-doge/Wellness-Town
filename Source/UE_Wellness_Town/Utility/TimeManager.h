// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TimeManager.generated.h"

/**
 * 
 */
UCLASS()
class UE_WELLNESS_TOWN_API UTimeManager : public UObject
{
	GENERATED_BODY()
public:

	void Update(float deltaTime);
	void UpdateHoursAndMins();
	int GetHours();
	int GetMinutes();

public:
	float timer;

	int hours;
	int minutes;
};
