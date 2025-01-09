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
class AItem;
class USplineComponent;
class UNiagaraComponent;
class UNiagaraSystem;

UCLASS()
class UE_WELLNESS_TOWN_API ANPC_Base : public ACharacter, public IInteractable
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANPC_Base();

	UFUNCTION(BlueprintCallable)
	void SetDestination(FVector destination);
	UFUNCTION(BlueprintCallable)
	FVector GetCurrentDestination();

	// Pauses movement in the controller. Used only for AI related actions (such as fishing). Pause AI instead if you want to prevent moving in general. false resumes movement.
	UFUNCTION(BlueprintCallable)
	void SetPauseMovement(bool val);
	// Checks if the NPC has a path
	UFUNCTION(BlueprintCallable)
	bool HasPath();

	// Sets it the AI is paused
	UFUNCTION(BlueprintCallable)
	void SetPauseAI(bool val);

	UFUNCTION(BlueprintCallable)
	void SetMood(Mood mood);

	void UseItem(USplineComponent* path);
	void PickUp(AItem* item);
	void DropItem();

	bool IsInPlayerRange();

	USplineComponent* GetSplineComponent();

	UFUNCTION(BlueprintCallable, Category = "NPC Data")
	UNPC_Data* GetNPCData();

	void ToggleVisibility(bool val);

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

	UPROPERTY()
	TObjectPtr<AItem> _heldObject;

	UPROPERTY()
	TObjectPtr<USplineComponent> _splineComponent;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraComponent> _niagara;
	UPROPERTY(EditAnywhere)
	TMap<Mood, TObjectPtr<UNiagaraSystem>> _niagaraSystems;

	UPROPERTY(EditAnywhere)
	Mood _startingMood;
	Mood _mood;

	UPROPERTY()
	TObjectPtr<APlayerCharacter> _player;
	UPROPERTY(EditAnywhere)
	float _hideDistanceFromPlayer;

	float _timer;

	FVector _currentDestination;
};
