// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Components/SphereComponent.h"
#include "UE_Wellness_Town/Interfaces/Interactable.h"
#include "UE_Wellness_Town/Characters/Player/PlayerMovementComponent.h"
#include "UE_Wellness_Town/Objects/MoveableActor.h"
#include "UE_Wellness_Town/Objects/Item.h"

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

	_splineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Spline Component"));
	checkf(_splineComponent, TEXT("Spline Component is an invalid value"));

	_heldObject = nullptr;
}

UCameraComponent* APlayerCharacter::GetCamera()
{
	return _camera;
}

void APlayerCharacter::PickUp(AItem* actor)
{
	if (_heldObject != nullptr)
	{
		DropHeldItem();
	}

	//Attaches to player and disables collision
	_heldObject = actor;
	_heldObject->DisableCollision();
	actor->SetActorLocation(GetMesh()->GetSocketLocation("HoldSocket"));
	actor->K2_AttachToComponent(GetMesh(), "HoldSocket", EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, true);
}

void APlayerCharacter::DropHeldItem()
{
	if (_heldObject == nullptr)
	{
		return;
	}

	//Reenables collision and detaches from player
	Cast<AItem>(_heldObject)->EnableCollision();
	_heldObject->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	_heldObject = nullptr;

	ClearSpline();
	_drawTrajectory = false;
}

void APlayerCharacter::CastHeldItem()
{
	if (_heldObject == nullptr || _heldObject->IsCastable() == false)
	{
		return;
	}

	if (_heldObject->DisplaySpline() == false)
	{
		bool success = _heldObject->ItemCast(this, nullptr);
		return;
	}

	//If the spline is empty (player didn't hold to display trajectory), create one
	if (_splineComponent->GetNumberOfSplinePoints() == 0)
	{
		FVector unitDirection = GetActorForwardVector() + GetActorUpVector();
		unitDirection.Normalize();


		CreateSpline(GetMesh()->GetSocketLocation("HoldSocket"), unitDirection, _throwStrength, 2, true);
	}

	bool success = _heldObject->ItemCast(this, _splineComponent);

	_splineComponent->ClearSplinePoints();

	for (TObjectPtr<AActor> splineMesh : _splineMeshes)
	{
		splineMesh->Destroy();
	}

	_splineMeshes.Empty();

	_drawTrajectory = false;
}

void APlayerCharacter::IsReadyToCast()
{
	if (_heldObject == nullptr || _heldObject->IsCastable() == false || _heldObject->DisplaySpline() == false)
	{
		return;
	}

	_drawTrajectory = true;
}

void APlayerCharacter::ThrowHeldItem()
{
	if (_heldObject == nullptr)
	{
		return;
	}

	ClearSpline();

	//Reenables collision and detaches from player, then applies physics to throw the item
	_heldObject->EnableCollision();
	_heldObject->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	FVector unitDirection = GetActorForwardVector() + GetActorUpVector();
	unitDirection.Normalize();

	_heldObject->GetStaticMesh()->SetPhysicsLinearVelocity(unitDirection * _throwStrength);
}

void APlayerCharacter::IsReadyToThrow()
{
	if (_heldObject == nullptr)
	{
		return;
	}

	_drawTrajectory = true;
}

void APlayerCharacter::Interact()
{
	if (IsAltInteracting == true)
	{
		_movementComponent->bOrientRotationToMovement = true;
		IsAltInteracting = false;
		return;
	}

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

	IsAltInteracting = true;

	//Disables player rotation if alt interacting, makes the pulling mechanic cleaner
	_movementComponent->bOrientRotationToMovement = false;
	_movementComponent->IsPushingActor()->AddForce(this);
}

float APlayerCharacter::GetMaxSpeed()
{
	return _movementComponent->MaxWalkSpeed;
}

void APlayerCharacter::EnableMovement()
{
	Cast<UPlayerMovementComponent>(GetMovementComponent())->EnableMovement();
}

void APlayerCharacter::DisableMovement()
{
	Cast<UPlayerMovementComponent>(GetMovementComponent())->DisableMovement();
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

void APlayerCharacter::DrawTrajectory()
{
	if (_lastLocation == GetActorLocation() && _lastRotation == GetActorRotation())
	{
		return;
	}

	for (TObjectPtr<AActor> splineMesh : _splineMeshes)
	{
		splineMesh->Destroy();
	}

	_splineMeshes.Empty();

	_lastLocation = GetActorLocation();
	_lastRotation = GetActorRotation();

	FVector start = GetMesh()->GetSocketLocation("HoldSocket");
	FVector unitDirection = GetActorForwardVector() + GetActorUpVector();
	unitDirection.Normalize();

	CreateSpline(start, unitDirection, _throwStrength, 3, false);
	
	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	for (int i = 0; i < _splineComponent->GetNumberOfSplinePoints(); i++)
	{
		TObjectPtr<AActor> mesh = GetWorld()->SpawnActor<AActor>(*_splineMesh, _splineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World), FRotator::ZeroRotator, spawnParams);
		_splineMeshes.Add(mesh);
	}
}

void APlayerCharacter::CreateSpline(FVector start, FVector unitDirection, float strength, int simTime, bool toDebug)
{
	_splineComponent->ClearSplinePoints();

	FPredictProjectilePathParams params;
	params.StartLocation = start;
	params.LaunchVelocity = unitDirection * strength;
	params.MaxSimTime = simTime;

	if (toDebug == true)
	{
		params.DrawDebugType = EDrawDebugTrace::ForDuration;
		params.DrawDebugTime = 1;
	}

	FPredictProjectilePathResult result;
	UGameplayStatics::PredictProjectilePath(_heldObject, params, result);

	for (int i = 0; i < result.PathData.Num(); i++)
	{
		_splineComponent->AddSplinePoint(result.PathData[i].Location, ESplineCoordinateSpace::World, true);
	}

	_splineComponent->UpdateSpline();
}

void APlayerCharacter::ClearSpline()
{
	_splineComponent->ClearSplinePoints();

	for (TObjectPtr<AActor> splineMesh : _splineMeshes)
	{
		splineMesh->Destroy();
	}

	_splineMeshes.Empty();

	_drawTrajectory = false;
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	_timer += DeltaTime;

	GEngine->AddOnScreenDebugMessage(1, 10, FColor::Red, FString::Printf(TEXT("CurrentTarget: %s"), _currentInteractTarget ? *_currentInteractTarget->GetName() : TEXT("NONE")));

	// Periodically checks if the trajectory should be drawn and checks whether or not the player is near interactable actors
	if (_timer < 0.05f)
	{
		return;
	}

	if (_drawTrajectory == true)
	{
		DrawTrajectory();
		_timer = 0;
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

