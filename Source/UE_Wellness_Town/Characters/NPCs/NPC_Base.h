// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UE_Wellness_Town/Interfaces/Interactable.h"
#include "NPC_Base.generated.h"

class AAIController;
class UNPC_Data;
class APlayerCharacter;
class UDialogueSystem;
class UGOAP_Agent;
class UStaticMeshComponent;

UCLASS()
class UE_WELLNESS_TOWN_API ANPC_Base : public ACharacter, public IInteractable
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANPC_Base();

	void LookAtTarget(AActor* target);
	void SetDestination(FVector destination);
	bool HasPath();

	UFUNCTION(BlueprintCallable, Category = "NPC Data")
	UNPC_Data* GetNPCData();

	virtual void Interact(APlayerCharacter* player) override;
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

	// Dialogue component default reference
	UPROPERTY(EditAnywhere)
	TSubclassOf<UDialogueSystem> _dialogueDefault;

	// Created copy of the default reference, this setup was just for testing and should be changed/removed if unneccessary 
	UPROPERTY(EditAnywhere)
	TObjectPtr<UDialogueSystem> _dialogueComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UGOAP_Agent> _agent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<AAIController> _controller;
};
