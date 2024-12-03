// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NPC_Data.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_WELLNESS_TOWN_API UNPC_Data : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UNPC_Data();

	int GetTrustScore();
	void AdjustTrustScore(int amount);

	int GetFriendshipScore();
	void AdjustFriendshipScore(int amount);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, meta = (ClampMin = 0, ClampMax = 10))
	int trust;

	UPROPERTY(EditAnywhere, meta = (ClampMin = 0, ClampMax = 10))
	int friendship;
};
