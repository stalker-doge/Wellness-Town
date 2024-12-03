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
	//void HandleLook(const struct FInputActionValue& value);
	void HandleMove(const struct FInputActionValue& value);
	void HandleJump();
	void HandleInteract();
	void HandleAltInteract();

	virtual void OnPossess(APawn* aPawn) override;
	virtual void OnUnPossess() override;

private:
	void BindActions(UEnhancedInputComponent* inputComponent);

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input | Mapping Context")
	TObjectPtr<UInputMappingContext> inputMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input | Character Movement")
	TObjectPtr<UInputAction> actionMove;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input | Character Movement")
	TObjectPtr<UInputAction> actionJump;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input | Character Actions")
	TObjectPtr<UInputAction> actionInteract;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input | Character Actions")
	TObjectPtr<UInputAction> actionAltInteract;

private:
	UPROPERTY()
	TObjectPtr<UEnhancedInputComponent> _enhancedInputComponent;

	UPROPERTY()
	TObjectPtr<APlayerCharacter> _playerCharacter;

	UPROPERTY()
	TObjectPtr<UCharacterMovementComponent> _movementComponent;
};
