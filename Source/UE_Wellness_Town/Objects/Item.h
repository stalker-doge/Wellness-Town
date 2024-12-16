// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UE_Wellness_Town/Interfaces/Interactable.h"
#include "Item.generated.h"

class APlayerCharacter;
class UStaticMeshComponent;
class USplineComponent;

UCLASS()
class UE_WELLNESS_TOWN_API AItem : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

	TObjectPtr<UStaticMeshComponent> GetStaticMesh();

	virtual void DisableCollision();
	void EnableCollision();

	bool IsCastable() { return _isCastable; };
	bool DisplaySpline() { return _displaySpline; };

	virtual void Interact(APlayerCharacter* player) override;
	virtual bool ItemCast(AActor* player, USplineComponent* path);

	// Called every frame
	virtual void Tick(float DeltaTime) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> _staticMesh;

	UPROPERTY(EditAnywhere)
	bool _isCastable;
	bool _displaySpline;
};
