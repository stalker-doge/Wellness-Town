// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SleepingSystem.generated.h"

/**
 * 
 */

UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UE_WELLNESS_TOWN_API USleepingSystem: public UActorComponent
{
	GENERATED_BODY()

	protected:
	// Called when the game starts
	virtual void BeginPlay() override;
public:
	USleepingSystem();
	~USleepingSystem();

	enum class TimeOfDay//enum for the time of day
	{
		Day,
		Night
	};
	TimeOfDay CurrentTime;//current time of day

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sleeping")//current day
		int CurrentDay;

	UFUNCTION(BlueprintCallable, Category = "Sleeping")//sets the current day
	void SetCurrentDay(int NewDay);

	UFUNCTION(BlueprintCallable, Category = "Sleeping")//gets the current day
	int GetCurrentDay();

	UFUNCTION(BlueprintCallable, Category = "Sleeping")//the actual sleeping function that changes the time of day
	void Sleep();

};
