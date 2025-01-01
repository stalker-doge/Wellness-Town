// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UE_Wellness_Town/Objects/Item.h"
#include "BugNet.generated.h"

class APlayerCharacter;
class USphereComponent;

UCLASS()
class UE_WELLNESS_TOWN_API ABugNet : public AItem
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ABugNet();

	virtual bool ItemCast(AActor* player, USplineComponent* path, bool isPlayer) override;
	virtual void DisableCollision() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	TObjectPtr<APlayerCharacter> _player;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USphereComponent> _collisionSphere;
};
