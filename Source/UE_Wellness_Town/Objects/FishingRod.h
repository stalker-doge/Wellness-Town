// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UE_Wellness_Town/Objects/Item.h"
#include "FishingRod.generated.h"

class USplineComponent;
class APlayerCharacter;
class ADestroyOnCollisionActor;
class ANPC_Base;

UCLASS()
class UE_WELLNESS_TOWN_API AFishingRod : public AItem
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFishingRod();

	void InitFishing();
	void CatchLoot();
	virtual bool ItemCast(AActor* player, USplineComponent* path, bool isPlayer) override;

	bool IsFishing();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<ADestroyOnCollisionActor>> _loot;

	TWeakObjectPtr<AActor> _owner;

	FVector _end;

	bool _isFishing;
	bool _isPlayer;
	bool _lootSpawned;
	float _timer;
};
