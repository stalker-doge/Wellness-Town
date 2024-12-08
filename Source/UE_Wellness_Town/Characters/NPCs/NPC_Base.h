// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UE_Wellness_Town/Interfaces/Interactable.h"
#include "NPC_Base.generated.h"

class UNPC_Data;
class APlayerCharacter;

UCLASS()
class UE_WELLNESS_TOWN_API ANPC_Base : public ACharacter, public IInteractable
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANPC_Base();

	TObjectPtr<UNPC_Data> GetNPCData();

	virtual void Interact(TObjectPtr<APlayerCharacter> player) override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UNPC_Data> _data;
};
