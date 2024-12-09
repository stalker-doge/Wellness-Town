// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMovementComponent.h"
#include "PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "UE_Wellness_Town/Animation/MantleAnimNotify.h"
#include "UE_Wellness_Town/Objects/MoveableActor.h"

void UPlayerMovementComponent::InitAnimations()
{
	if (const auto MantleNotify = FindNotifyByClass<UMantleAnimNotify>(_mantleAnimation))
	{
		MantleNotify->OnNotified.AddUObject(this, &UPlayerMovementComponent::OnMantleAnimFinished);
	}
}

void UPlayerMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();

	_player = Cast<APlayerCharacter>(GetOwner());
	check(_player);

	InitAnimations();
}

void UPlayerMovementComponent::PhysCustom(float deltaTime, int32 Iterations)
{
	Super::PhysCustom(deltaTime, Iterations);

	switch (CustomMovementMode)
	{
	case CMOVE_Pushing:
		PhysPushing(deltaTime, Iterations);
		break;
	}
}

void UPlayerMovementComponent::UpdateCharacterStateBeforeMovement(float DeltaSeconds)
{
	GEngine->AddOnScreenDebugMessage(3, 10, FColor::Red, FString::Printf(TEXT("Can Push: %s "), _isPushing ? TEXT("TRUE") : TEXT("FALSE")));
	TryPushing();
	TryMantle();

	Super::UpdateCharacterStateBeforeMovement(DeltaSeconds);
}

void UPlayerMovementComponent::UpdateCharacterStateAfterMovement(float DeltaSeconds)
{
	Super::UpdateCharacterStateAfterMovement(DeltaSeconds);

	const TSharedPtr<FRootMotionSource> RootMotion = GetRootMotionSourceByID(RootMotionSourceID);

	if (RootMotion && RootMotion->Status.HasFlag(ERootMotionSourceStatusFlags::Finished))
	{
		RemoveRootMotionSourceByID(RootMotionSourceID);
	}
}

float UPlayerMovementComponent::GetSpeed() const
{
	return Velocity.Length();
}

bool UPlayerMovementComponent::IsWalk() const
{
	if (!IsWalking() || FMath::IsNearlyZero(GetSpeed()))
	{
		return false;
	}

	if (IsPushing())
	{
		return false;
	}

	return true;
}

#pragma region "Pushing"
bool UPlayerMovementComponent::CanPush()
{
	if (_player->IsAltInteracting == false)
	{
		//return false;
	}

	// check if the character is on walkable floor
	FFindFloorResult FloorResult;
	FindFloor(UpdatedComponent->GetComponentLocation(), FloorResult, false);

	if (!FloorResult.bWalkableFloor)
	{
		_pushingActor = nullptr;
		return false;
	}

	// check if exist movable actor in front of the character
	FHitResult HitResult;
	FCollisionQueryParams params;
	params.AddIgnoredActor(GetOwner());

	const FVector Start = UpdatedComponent->GetComponentLocation();
	const FVector End = Start + UpdatedComponent->GetForwardVector() * ForwardSearchPushingLength;

	if (!GetWorld()->LineTraceSingleByProfile(HitResult, Start, End, "BlockAll", params))
	{
		_pushingActor = nullptr;
		return false;
	}

	const TObjectPtr<AActor> Actor = HitResult.GetActor();
	if (!Actor)
	{
		_pushingActor = nullptr;
		return false;
	}

	if (Actor->ActorHasTag(MovableTag) == false)
	{
		_pushingActor = nullptr;
		return false;
	}

	_pushingActor = Cast<AMoveableActor>(Actor);
	return true;
}

void UPlayerMovementComponent::TryPushing()
{
	_isPushing = CanPush();
	if (_isPushing == false)
	{
		return;
	}

	// change capsule size to pushing dimensions
	const float OldUnscaleHalfHeight = CharacterOwner->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
	CharacterOwner->GetCapsuleComponent()->SetCapsuleSize(PushingCapsuleRadius, OldUnscaleHalfHeight);
	SetMovementMode(MOVE_Custom, CMOVE_Pushing);
}

void UPlayerMovementComponent::PhysPushing(const float DeltaTime, const int32 Iterations)
{
	if (_isPushing == false)
	{
		// restore default capsule size
		const TObjectPtr<ACharacter> DefaultCharacter = CharacterOwner->GetClass()->GetDefaultObject<ACharacter>();
		const float DefaultUnscaleRadius = DefaultCharacter->GetCapsuleComponent()->GetUnscaledCapsuleRadius();
		const float DefaultUnscaleHalfHeight = DefaultCharacter->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
		CharacterOwner->GetCapsuleComponent()->SetCapsuleSize(DefaultUnscaleRadius, DefaultUnscaleHalfHeight);
		SetMovementMode(DefaultLandMovementMode);
	}

	PhysWalking(DeltaTime, Iterations);
}

TObjectPtr<AMoveableActor> UPlayerMovementComponent::IsPushingActor()
{
	return _pushingActor;
}

#pragma endregion
#pragma region "Mantling"

void UPlayerMovementComponent::TryMantle()
{
	if (IsFalling() == false)
	{
		return;
	}

	float minWallSteepnessAngleCos = FMath::Cos(FMath::DegreesToRadians(_mantleMinWallSteepnessAngle));
	float maxSurfaceAngleCos = FMath::Cos(FMath::DegreesToRadians(_mantleMaxSurfaceAngle));
	float maxAlignmentAngleCos = FMath::Cos(FMath::DegreesToRadians(_mantleMaxAlignmentAngle));

	FVector componentLocation = UpdatedComponent->GetComponentLocation();
	FCollisionQueryParams params;
	params.AddIgnoredActor(GetOwner());

	FHitResult frontResult;
	FVector frontStart = componentLocation + FVector::UpVector * _mantleUpOffsetDistance;
	FVector forwardVector = UpdatedComponent->GetForwardVector().GetSafeNormal();

	float distance = FMath::Clamp(Velocity | forwardVector, GetCapsuleRadius() + 30.0f, _maxFrontMantleCheckDistance);
	FVector frontEnd = frontStart + forwardVector * distance;

	DrawDebugLine(GetWorld(), frontStart, frontEnd, FColor::Red);

	if (GetWorld()->LineTraceSingleByProfile(frontResult, frontStart, frontEnd, "BlockAll", params) == false)
	{
		return;
	}
	if (frontResult.IsValidBlockingHit() == false)
	{
		return;
	}

	float cosWallSteepness = frontResult.Normal | FVector::UpVector;
	if (FMath::Abs(cosWallSteepness) > minWallSteepnessAngleCos || (forwardVector | -frontResult.Normal) < maxAlignmentAngleCos)
	{
		return;
	}
	
	DrawDebugPoint(GetWorld(), frontResult.Location, 30, FColor::Emerald, false, 2);

	TArray<FHitResult> heightHits;
	FHitResult surfaceHits;
	FVector wallUp = FVector::VectorPlaneProject(FVector::UpVector, frontResult.Normal).GetSafeNormal();
	float wallCos = FVector::UpVector | frontResult.Normal;
	float wallSin = FMath::Sqrt(1 - wallCos * wallCos);

	FVector traceStart = frontResult.Location + forwardVector + wallUp * _mantleReachHeight / wallSin;
	DrawDebugLine(GetWorld(), traceStart, frontResult.Location + forwardVector, FColor::Black);

	if (GetWorld()->LineTraceMultiByProfile(heightHits, traceStart, frontResult.Location + forwardVector, "BlockAll", params) == false)
	{
		return;
	}

	for (const FHitResult& hit : heightHits)
	{
		if (hit.IsValidBlockingHit())
		{
			surfaceHits = hit;
			break;
		}
	}

	if (surfaceHits.IsValidBlockingHit() == false || (surfaceHits.Normal | FVector::UpVector) < maxSurfaceAngleCos)
	{
		return;
	}

	float height = surfaceHits.Location - componentLocation | FVector::UpVector;
	DrawDebugPoint(GetWorld(), surfaceHits.Location, 20, FColor::Blue, false, 2);

	if(height > _mantleReachHeight)
	{
		return;
	}

	float surfaceCos = FVector::UpVector | surfaceHits.Normal;
	float surfaceSin = FMath::Sqrt(1 - surfaceCos * surfaceCos);
	FVector transitionTarget = surfaceHits.Location + forwardVector + GetCapsuleRadius() + FVector::UpVector * (GetCapsuleHalfHeight() + 1 + GetCapsuleRadius() * 2 * surfaceSin);
	FCollisionShape capShape = FCollisionShape::MakeCapsule(GetCapsuleRadius(), GetCapsuleHalfHeight());

	if (GetWorld()->OverlapAnyTestByProfile(transitionTarget, FQuat::Identity, "BlockAll", capShape, params) == true)
	{
		return;
	}

	DrawDebugCapsule(GetWorld(), componentLocation, GetCapsuleHalfHeight(), GetCapsuleRadius(), FQuat::Identity, FColor::Red, false, 4);
	DrawDebugCapsule(GetWorld(), transitionTarget, GetCapsuleHalfHeight(), GetCapsuleRadius(), FQuat::Identity, FColor::Green, false, 3);

	RootMotionSource.Reset();
	RootMotionSource = MakeShared<FRootMotionSource_MoveToForce>();
	RootMotionSource->AccumulateMode = ERootMotionAccumulateMode::Override;
	RootMotionSource->Duration = _mantleAnimation->GetPlayLength() - 0.1f;
	RootMotionSource->StartLocation = UpdatedComponent->GetComponentLocation();
	RootMotionSource->TargetLocation = transitionTarget;

	Acceleration = FVector::ZeroVector;
	Velocity = FVector::ZeroVector;;

	SetMovementMode(MOVE_Flying);
	RootMotionSourceID = ApplyRootMotionSource(RootMotionSource);


	_player->PlayAnimMontage(_mantleAnimation);
}

void UPlayerMovementComponent::OnMantleAnimFinished()
{
	SetMovementMode(DefaultLandMovementMode);
}

#pragma endregion


bool UPlayerMovementComponent::IsCustomMovementMode(const ECustomMovementMode InCustomMovementMode) const
{
	return MovementMode == MOVE_Custom && CustomMovementMode == InCustomMovementMode;
}

float UPlayerMovementComponent::GetCapsuleRadius() const
{
	return CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleRadius();
}

float UPlayerMovementComponent::GetCapsuleHalfHeight() const
{
	return CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
}

float UPlayerMovementComponent::GetMaxSpeed() const
{
	const float maxSpeed = Super::GetMaxSpeed();

	if (MovementMode != MOVE_Custom)
	{
		return maxSpeed;
	}

	switch (CustomMovementMode)
	{
	case CMOVE_Pushing:
		return MaxPushingSpeed;
	default:
		return maxSpeed;
	}
}
