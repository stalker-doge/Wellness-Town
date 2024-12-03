// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SphereComponent.h"
#include "UE_Wellness_Town/Interfaces/Interactable.h"
#include "UE_Wellness_Town/Characters/PlayerMovementComponent.h"
#include "UE_Wellness_Town/Objects/MoveableActor.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_springArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	checkf(_springArmComponent, TEXT("Spring Arm is an invalid value"));

	_springArmComponent->SetupAttachment(RootComponent);

	_camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
	checkf(_camera, TEXT("PLAYER_CAMERA is an invalid value"));

	_camera->AttachToComponent(_springArmComponent, FAttachmentTransformRules::KeepRelativeTransform);

	_springArmComponent->bUsePawnControlRotation = true;
	_springArmComponent->bEnableCameraLag = true;
	_springArmComponent->TargetArmLength = 300;

	_interactCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Interact Collider"));
	checkf(_interactCollider, TEXT("Interact Collided is an invalid value"));

	_interactCollider->SetCollisionProfileName("Interact");
	_interactCollider->SetupAttachment(RootComponent);
}

TObjectPtr<UCameraComponent> APlayerCharacter::GetCamera()
{
	return _camera;
}

void APlayerCharacter::Interact()
{
	if (_currentInteractTarget == nullptr)
	{
		return;
	}

	IInteractable* interact = Cast<IInteractable>(_currentInteractTarget);

	if (interact == nullptr)
	{
		return;
	}

	interact->Interact(this);
}

void APlayerCharacter::AltInteract()
{
	if (_movementComponent->IsPushing() == false || _movementComponent->IsPushingActor() == nullptr)
	{
		return;
	}

	FRotator newRotation = (_movementComponent->IsPushingActor()->GetActorLocation() - GetActorLocation()).Rotation();
	SetActorRotation(newRotation);

	_movementComponent->IsPushingActor()->AddForce(this);
}

void APlayerCharacter::BeginInteractOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (_currentInteractTarget == nullptr)
	{
		_currentInteractTarget = OtherActor;
	}
}

void APlayerCharacter::EndInteractOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == _currentInteractTarget)
	{
		_currentInteractTarget = nullptr;
	}
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	_movementComponent = Cast<UPlayerMovementComponent>(GetMovementComponent());
	checkf(_movementComponent, TEXT("Movement Component is an invalid value"));

	_interactCollider->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::BeginInteractOverlap);
	_interactCollider->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::EndInteractOverlap);
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	_timer += DeltaTime;

	GEngine->AddOnScreenDebugMessage(1, 10, FColor::Red, FString::Printf(TEXT("CurrentTarget: %s"), _currentInteractTarget ? *_currentInteractTarget->GetName() : TEXT("NONE")));

	if (_timer < 0.1f)
	{
		return;
	}

	if (_interactCollider->GetOverlapInfos().Num() <= 1)
	{
		return;
	}

	_timer = 0;

	TArray<AActor*> collidingWith; 
	_interactCollider->GetOverlappingActors(collidingWith);

	for (AActor* actor : collidingWith)
	{
		if (actor == _currentInteractTarget)
		{
			continue;
		}

		if (_currentInteractTarget == nullptr)
		{
			_currentInteractTarget = actor;
			continue;
		}

		FVector distToCurrent = GetActorLocation() - _currentInteractTarget->GetActorLocation();
		FVector distToNew = GetActorLocation() - actor->GetActorLocation();

		if (distToNew.Length() < distToCurrent.Length())
		{
			_currentInteractTarget = actor;
		}
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

