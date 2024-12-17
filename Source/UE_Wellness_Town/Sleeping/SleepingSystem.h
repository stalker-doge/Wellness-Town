// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class UE_WELLNESS_TOWN_API SleepingSystem
{
public:
	SleepingSystem();
	~SleepingSystem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sleeping")//enum for time of day
		enum TimeOfDay
	{
		Day,
		Night
	};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sleeping")//current time of day
		TimeOfDay CurrentTime{ TimeOfDay::Day };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sleeping")//current day
		int CurrentDay;

	UFUNCTION(BlueprintCallable, Category = "Sleeping")
	void SetCurrentDay(int NewDay);

	UFUNCTION(BlueprintCallable, Category = "Sleeping")
	int GetCurrentDay();

	UFUNCTION(BlueprintCallable, Category = "Sleeping")
	void Sleep();

};
