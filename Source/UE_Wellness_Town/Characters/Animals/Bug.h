// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Bug.generated.h"

class UStaticMeshComponent;
class UNavigationSystemV1;
class AAIController;

UCLASS()
class UE_WELLNESS_TOWN_API ABug : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABug();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> _bugMesh;
	TObjectPtr<UNavigationSystemV1> _navSystem;

	TObjectPtr<AAIController> _controller;
};
