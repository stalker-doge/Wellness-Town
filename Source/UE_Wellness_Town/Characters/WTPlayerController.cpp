// Fill out your copyright notice in the Description page of Project Settings.


#include "WTPlayerController.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "PlayerCharacter.h"

//void AWTPlayerController::HandleLook(const FInputActionValue& value)
//{
//}

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

void AWTPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	_playerCharacter = Cast<APlayerCharacter>(aPawn);
	checkf(_playerCharacter, TEXT("PLAYER_CHARACTER is an invalid value"));

	_movementComponent = _playerCharacter->GetCharacterMovement();
	checkf(_movementComponent, TEXT("MOVEMENT_COMPONENT is an invalid value"));

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

void AWTPlayerController::BindActions(UEnhancedInputComponent* inputComponent)
{
	if (actionMove != nullptr)
	{
		inputComponent->BindAction(actionMove, ETriggerEvent::Triggered, this, &AWTPlayerController::HandleMove);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Move Action"));
	}

	if (actionInteract != nullptr)
	{
		inputComponent->BindAction(actionInteract, ETriggerEvent::Triggered, this, &AWTPlayerController::HandleInteract);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Interact Action"));
	}

	if (actionAltInteract != nullptr)
	{
		inputComponent->BindAction(actionAltInteract, ETriggerEvent::Triggered, this, &AWTPlayerController::HandleAltInteract);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Interact Action"));
	}

	if (actionJump != nullptr)
	{
		inputComponent->BindAction(actionJump, ETriggerEvent::Triggered, this, &AWTPlayerController::HandleJump);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Jump Action"));
	}
}
