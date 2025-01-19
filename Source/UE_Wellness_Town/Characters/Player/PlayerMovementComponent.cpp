// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMovementComponent.h"
#include "PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "UE_Wellness_Town/Animation/MantleAnimNotify.h"
#include "UE_Wellness_Town/Objects/MoveableActor.h"


void UPlayerMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();

	_player = Cast<APlayerCharacter>(GetOwner());
	check(_player);

	_lastMode = EMovementMode::MOVE_Walking;
	InitAnimations();
}

#pragma region "Movement"
void UPlayerMovementComponent::UpdateCharacterStateBeforeMovement(float DeltaSeconds)
{
	TryPushing();
	TryMantle();

	Super::UpdateCharacterStateBeforeMovement(DeltaSeconds);
}

void UPlayerMovementComponent::UpdateCharacterStateAfterMovement(float DeltaSeconds)
{
	Super::UpdateCharacterStateAfterMovement(DeltaSeconds);

	const TSharedPtr<FRootMotionSource> rootMotion = GetRootMotionSourceByID(_rootMotionSourceID);

	if (rootMotion && rootMotion->Status.HasFlag(ERootMotionSourceStatusFlags::Finished))
	{
		RemoveRootMotionSourceByID(_rootMotionSourceID);
	}
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

void UPlayerMovementComponent::EnableMovement()
{
	MovementMode = _lastMode;
}

void UPlayerMovementComponent::DisableMovement()
{
	_lastMode = MovementMode;
	MovementMode = EMovementMode::MOVE_None;
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

	//Checks if a valid mantleable object is ahead of the player 
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

	//Checks if the object has a reachable point on top of it
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

	if (height > _mantleReachHeight)
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

	//Starts the animation and applies a fixed motion to the player
	_rootMotionSource.Reset();
	_rootMotionSource = MakeShared<FRootMotionSource_MoveToForce>();
	_rootMotionSource->AccumulateMode = ERootMotionAccumulateMode::Override;
	_rootMotionSource->Duration = _mantleAnimation->GetPlayLength() - 0.1f;
	_rootMotionSource->StartLocation = UpdatedComponent->GetComponentLocation();
	_rootMotionSource->TargetLocation = transitionTarget;

	Acceleration = FVector::ZeroVector;
	Velocity = FVector::ZeroVector;;

	SetMovementMode(MOVE_Flying);
	_rootMotionSourceID = ApplyRootMotionSource(_rootMotionSource);


	_player->PlayAnimMontage(_mantleAnimation);
}

void UPlayerMovementComponent::OnMantleAnimFinished()
{
	SetMovementMode(DefaultLandMovementMode);
}

#pragma endregion

#pragma region "Pushing"
void UPlayerMovementComponent::TryPushing()
{
	_isPushing = CanPush();
	if (_isPushing == false)
	{
		return;
	}

	// Change capsule size to pushing dimensions
	const float oldUnscaleHalfHeight = CharacterOwner->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
	CharacterOwner->GetCapsuleComponent()->SetCapsuleSize(PushingCapsuleRadius, oldUnscaleHalfHeight);
	SetMovementMode(MOVE_Custom, CMOVE_Pushing);
}

bool UPlayerMovementComponent::CanPush()
{
	// Check if the character is on walkable floor
	FFindFloorResult floorResult;
	FindFloor(UpdatedComponent->GetComponentLocation(), floorResult, false);

	if (!floorResult.bWalkableFloor)
	{
		_pushedActor = nullptr;
		return false;
	}

	// Check if a movable actor is in front of the player
	FHitResult hitResult;
	FCollisionQueryParams params;
	params.AddIgnoredActor(GetOwner());

	const FVector start = UpdatedComponent->GetComponentLocation();
	const FVector end = start + UpdatedComponent->GetForwardVector() * ForwardSearchPushingLength;

	if (!GetWorld()->LineTraceSingleByProfile(hitResult, start, end, "BlockAll", params))
	{
		_pushedActor = nullptr;
		return false;
	}

	const TObjectPtr<AActor> Actor = hitResult.GetActor();

	if (!Actor)
	{
		_pushedActor = nullptr;
		return false;
	}

	if (Actor->ActorHasTag(MovableTag) == false)
	{
		_pushedActor = nullptr;
		return false;
	}

	_pushedActor = Cast<AMoveableActor>(Actor);
	return true;
}

void UPlayerMovementComponent::PhysPushing(const float DeltaTime, const int32 Iterations)
{
	if (_isPushing == false)
	{
		// Restore default capsule size
		const TObjectPtr<ACharacter> defaultCharacter = CharacterOwner->GetClass()->GetDefaultObject<ACharacter>();
		const float defaultUnscaleRadius = defaultCharacter->GetCapsuleComponent()->GetUnscaledCapsuleRadius();
		const float defaultUnscaleHalfHeight = defaultCharacter->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
		CharacterOwner->GetCapsuleComponent()->SetCapsuleSize(defaultUnscaleRadius, defaultUnscaleHalfHeight);
		SetMovementMode(DefaultLandMovementMode);
	}

	PhysWalking(DeltaTime, Iterations);
}

AMoveableActor* UPlayerMovementComponent::GetPushedActor() const
{
	return _pushedActor;
}
#pragma endregion

#pragma region "Helper"

float UPlayerMovementComponent::GetSpeed() const
{
	return Velocity.Length();
}

bool UPlayerMovementComponent::IsWalkingCustom() const
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
bool UPlayerMovementComponent::IsPushing() const
{
	return _isPushing;
}

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
#pragma endregion

#pragma region "Animation"
void UPlayerMovementComponent::InitAnimations()
{
	if (const auto mantleNotify = FindNotifyByClass<UMantleAnimNotify>(_mantleAnimation))
	{
		mantleNotify->OnNotified.AddUObject(this, &UPlayerMovementComponent::OnMantleAnimFinished);
	}
}

template<typename AnimNotify>
inline AnimNotify* UPlayerMovementComponent::FindNotifyByClass(const TObjectPtr<UAnimSequenceBase> animation)
{
	if (animation == nullptr)
	{
		return nullptr;
	}

	for (FAnimNotifyEvent notifyEvent : animation->Notifies)
	{
		if (const auto AnimationNotify = Cast<AnimNotify>(notifyEvent.Notify))
		{
			return AnimationNotify;
		}
	}

	return nullptr;
}
#pragma endregion


