// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UE_Wellness_Town/Interfaces/Interactable.h"
#include "TestInteractable.generated.h"

class APlayerCharacter;
class UStaticMeshComponent;

UCLASS()
class UE_WELLNESS_TOWN_API ATestInteractable : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestInteractable();

	virtual void Interact(TObjectPtr<APlayerCharacter> player) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> _staticMesh;
};
