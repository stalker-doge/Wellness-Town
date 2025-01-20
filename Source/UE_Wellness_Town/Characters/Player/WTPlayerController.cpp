// Fill out your copyright notice in the Description page of Project Settings.


#include "WTPlayerController.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"

void AWTPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	_playerCharacter = Cast<APlayerCharacter>(aPawn);
	_movementComponent = _playerCharacter->GetCharacterMovement();

	_enhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	checkf(_enhancedInputComponent, TEXT("ENHANCED_INPUT_COMPONENT is an invalid value"));

	UEnhancedInputLocalPlayerSubsystem* inputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	checkf(inputSubsystem, TEXT("INPUT_SUBSYSTEM is an invalid value"));

	checkf(inputMappingContext, TEXT("INPUT_MAPPING_CONTEXT is an invalid value"));
	inputSubsystem->ClearAllMappings();
	inputSubsystem->AddMappingContext(inputMappingContext, 0);

	BindActions(_enhancedInputComponent);
}

void AWTPlayerController::OnUnPossess()
{
	_enhancedInputComponent->ClearActionBindings();

	Super::OnUnPossess();
}

#pragma region "Inputs"
void AWTPlayerController::HandleLook(const FInputActionValue& value)
{
	const float movementVector = value.Get<float>();

	_playerCharacter->GetSpringArm()->AddLocalRotation(FRotator(0, movementVector, 0));
}

void AWTPlayerController::HandleMove(const FInputActionValue& value)
{
	const FVector movementVector = value.Get<FVector>();

	_playerCharacter->AddMovementInput(_playerCharacter->GetCamera()->GetForwardVector(), movementVector.Y);
	_playerCharacter->AddMovementInput(_playerCharacter->GetCamera()->GetRightVector(), movementVector.X);
}

void AWTPlayerController::HandleJump()
{
	_playerCharacter->UnCrouch();
	_playerCharacter->Jump();
}

void AWTPlayerController::HandleInteract()
{
	_playerCharacter->Interact();
}

void AWTPlayerController::HandleAltInteract()
{
	_playerCharacter->AltInteract();
}

void AWTPlayerController::HandleDrop()
{
	_playerCharacter->DropHeldItem();
}

void AWTPlayerController::HandleCast()
{
	_playerCharacter->CastHeldItem();
}

void AWTPlayerController::HandleCastHold()
{
	_playerCharacter->IsReadyToCast();
}

void AWTPlayerController::HandleThrow()
{
	_playerCharacter->ThrowHeldItem();
}

void AWTPlayerController::HandleThrowHold()
{
	_playerCharacter->IsReadyToThrow();
}
#pragma endregion

void AWTPlayerController::BindActions(UEnhancedInputComponent* inputComponent)
{
	checkf(actionLook, TEXT("Missing Look Action"));
	inputComponent->BindAction(actionLook, ETriggerEvent::Triggered, this, &AWTPlayerController::HandleLook);

	checkf(actionMove, TEXT("Missing Move Action"));
	inputComponent->BindAction(actionMove, ETriggerEvent::Triggered, this, &AWTPlayerController::HandleMove);

	checkf(actionInteract, TEXT("Missing Interact Action"));
	inputComponent->BindAction(actionInteract, ETriggerEvent::Triggered, this, &AWTPlayerController::HandleInteract);

	checkf(actionAltInteract, TEXT("Missing AltInteract Action"));
	inputComponent->BindAction(actionAltInteract, ETriggerEvent::Triggered, this, &AWTPlayerController::HandleAltInteract);

	checkf(actionJump, TEXT("Missing Jump Action"));
	inputComponent->BindAction(actionJump, ETriggerEvent::Triggered, this, &AWTPlayerController::HandleJump);

	checkf(actionDrop, TEXT("Missing Drop Action"));
	inputComponent->BindAction(actionDrop, ETriggerEvent::Triggered, this, &AWTPlayerController::HandleDrop);

	checkf(actionThrow, TEXT("Missing Throw Action"));
	inputComponent->BindAction(actionThrow, ETriggerEvent::Triggered, this, &AWTPlayerController::HandleThrow);

	checkf(actionThrowHold, TEXT("Missing ThrowHold Action"));
	inputComponent->BindAction(actionThrowHold, ETriggerEvent::Triggered, this, &AWTPlayerController::HandleThrowHold);

	checkf(actionCast, TEXT("Missing Cast Action"));
	inputComponent->BindAction(actionCast, ETriggerEvent::Triggered, this, &AWTPlayerController::HandleCast);

	checkf(actionCastHold, TEXT("Missing CastHold Action"));
	inputComponent->BindAction(actionCastHold, ETriggerEvent::Triggered, this, &AWTPlayerController::HandleCastHold);
}
