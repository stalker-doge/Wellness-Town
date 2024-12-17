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

		enum class TimeOfDay
	{
		Day,
		Night
	};
		TimeOfDay CurrentTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sleeping")//current day
		int CurrentDay;

	UFUNCTION(BlueprintCallable, Category = "Sleeping")
	void SetCurrentDay(int NewDay);

	UFUNCTION(BlueprintCallable, Category = "Sleeping")
	int GetCurrentDay();

	UFUNCTION(BlueprintCallable, Category = "Sleeping")
	void Sleep();

};
