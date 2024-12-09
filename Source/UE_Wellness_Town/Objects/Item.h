// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UE_Wellness_Town/Interfaces/Interactable.h"
#include "Item.generated.h"

class APlayerCharacter;
class UStaticMeshComponent;

UCLASS()
class UE_WELLNESS_TOWN_API AItem : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

	TObjectPtr<UStaticMeshComponent> GetStaticMesh();

	void DisableCollision();
	void EnableCollision();

	virtual void Interact(TObjectPtr<APlayerCharacter> player) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> _staticMesh;
};
