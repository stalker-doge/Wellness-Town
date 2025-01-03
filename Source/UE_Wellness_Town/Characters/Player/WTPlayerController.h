// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "WTPlayerController.generated.h"

class UInputMappingContext;
class UEnhancedInputComponent;
class UInputAction;
class UCharacterMovementComponent;
class APlayerCharacter;

UCLASS()
class UE_WELLNESS_TOWN_API AWTPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	void HandleLook(const struct FInputActionValue& value);
	void HandleMove(const struct FInputActionValue& value);
	void HandleJump();
	void HandleInteract();
	void HandleAltInteract();
	void HandleDrop();
	void HandleCast();
	void HandleCastHold();
	void HandleThrow();
	void HandleThrowHold();

	virtual void OnPossess(APawn* aPawn) override;
	virtual void OnUnPossess() override;

private:
	void BindActions(UEnhancedInputComponent* inputComponent);

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input | Mapping Context")
	TObjectPtr<UInputMappingContext> inputMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input | Character Movement")
	TObjectPtr<UInputAction> actionLook;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input | Character Movement")
	TObjectPtr<UInputAction> actionMove;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input | Character Movement")
	TObjectPtr<UInputAction> actionJump;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input | Character Actions")
	TObjectPtr<UInputAction> actionInteract;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input | Character Actions")
	TObjectPtr<UInputAction> actionAltInteract;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input | Character Actions")
	TObjectPtr<UInputAction> actionDrop;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input | Character Actions")
	TObjectPtr<UInputAction> actionThrow;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input | Character Actions")
	TObjectPtr<UInputAction> actionThrowHold;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input | Character Actions")
	TObjectPtr<UInputAction> actionCast;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input | Character Actions")
	TObjectPtr<UInputAction> actionCastHold;

private:
	UPROPERTY()
	TObjectPtr<UEnhancedInputComponent> _enhancedInputComponent;

	UPROPERTY()
	TObjectPtr<APlayerCharacter> _playerCharacter;

	UPROPERTY()
	TObjectPtr<UCharacterMovementComponent> _movementComponent;
};
