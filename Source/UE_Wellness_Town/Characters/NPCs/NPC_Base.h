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

	void ToggleVisibility(bool val);

	//---- AI ----
	UFUNCTION(BlueprintCallable)
	void SetDestination(FVector destination);
	// Pauses movement in the controller. Used only for AI controlled actions (such as fishing). Pause AI instead if you want to prevent moving in general (for quest related reason, as an example). False resumes movement.
	UFUNCTION(BlueprintCallable)
	void SetPauseMovement(bool val);
	// Sets it the AI is paused
	UFUNCTION(BlueprintCallable)
	void SetPauseAI(bool val);
	UFUNCTION(BlueprintCallable)
	void SetMood(Mood mood);

	//---- ITEM ----
	void UseItem(USplineComponent* path);
	void PickUp(AItem* item);
	void DropItem();

	//---- HELPER ----
	UFUNCTION(BlueprintCallable, Category = "NPC Data")
	UNPC_Data* GetNPCData();
	UFUNCTION(BlueprintCallable)
	FVector GetCurrentDestination() const;
	USplineComponent* GetSplineComponent();
	UFUNCTION(BlueprintCallable)
	bool HasPath() const;
	bool IsInPlayerRange();

	virtual void Interact(APlayerCharacter* player) override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	//---- STATS ----
	UPROPERTY(EditDefaultsOnly)
	float _hideDistanceFromPlayer;

	//---- AI ----
	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UNPC_Data> _data;
	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UGOAP_Agent> _agent;
	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<AAIController> _controller;
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	Mood _startingMood;
	Mood _mood;

	//---- ITEM ----
	TWeakObjectPtr<AItem> _heldObject;
	UPROPERTY()
	TObjectPtr<USplineComponent> _splineComponent;

	//---- VFX ----
	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraComponent> _niagara;
	UPROPERTY(EditAnywhere)
	TMap<Mood, TObjectPtr<UNiagaraSystem>> _niagaraSystems;

	TWeakObjectPtr<APlayerCharacter> _player;

	FVector _currentDestination;

	float _timer;
};
