// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MoveableActor.generated.h"

class UPhysicsConstraintComponent;
class UStaticMeshComponent;
class APlayerCharacter; 

UCLASS()
class UE_WELLNESS_TOWN_API AMoveableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMoveableActor();

	void AddForce(TObjectPtr<APlayerCharacter> target);
private:
	UPROPERTY(EditAnywhere, Category = "Component")
	TObjectPtr<UStaticMeshComponent> _meshComponent;

	UPROPERTY(EditAnywhere, Category = "Component")
	TObjectPtr<UPhysicsConstraintComponent> _physicsComponent;
};
