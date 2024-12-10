// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerMovementComponent.generated.h"

class APlayerCharacter;
class AMoveableActor;

UENUM(BlueprintType)
enum ECustomMovementMode
{
	CMOVE_None UMETA(DisplayName = "None"),
	CMOVE_Pushing UMETA(DisplayName = "Pushing"),
	CMOVE_MAX UMETA(Hidden),
};

UCLASS()
class UE_WELLNESS_TOWN_API UPlayerMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:
	virtual void UpdateCharacterStateBeforeMovement(float DeltaSeconds) override;
	virtual void UpdateCharacterStateAfterMovement(float DeltaSeconds) override;

	// Blueprint Animation
	UFUNCTION(BlueprintPure) 
	float GetSpeed() const;
	UFUNCTION(BlueprintPure) 
	bool IsWalk() const;
	UFUNCTION(BlueprintPure) 
	bool IsPushing() const { return _isPushing; }

	void EnableMovement();
	void DisableMovement();

	//Mantling
	int RootMotionSourceID;
	TSharedPtr<FRootMotionSource_MoveToForce> RootMotionSource;
	void TryMantle();
	void OnMantleAnimFinished();

	void TryPushing();
	bool CanPush();
	void PhysPushing(float DeltaTime, int32 Iterations);
	TObjectPtr<AMoveableActor> IsPushingActor();

	bool IsCustomMovementMode(const ECustomMovementMode InCustomMovementMode) const;
	float GetCapsuleRadius() const;
	float GetCapsuleHalfHeight() const;
	virtual float GetMaxSpeed() const override;

	void InitAnimations();
	template<typename AnimNotify>
	AnimNotify* FindNotifyByClass(const TObjectPtr<UAnimSequenceBase> animation)
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

protected:
	virtual void InitializeComponent() override;
	virtual void PhysCustom(float deltaTime, int32 Iterations) override;

private:
	UPROPERTY()
	TObjectPtr<APlayerCharacter> _player;

	UPROPERTY(EditDefaultsOnly, Category = "Player Movement | Mantle")
	float _maxFrontMantleCheckDistance = 50;
	UPROPERTY(EditDefaultsOnly, Category = "Player Movement | Mantle")
	float _mantleUpOffsetDistance = 30;
	UPROPERTY(EditDefaultsOnly, Category = "Player Movement | Mantle")
	float _mantleReachHeight = 50;
	UPROPERTY(EditDefaultsOnly, Category = "Player Movement | Mantle")
	float _mantleMinWallSteepnessAngle = 75;
	UPROPERTY(EditDefaultsOnly, Category = "Player Movement | Mantle")
	float _mantleMaxSurfaceAngle = 40;
	UPROPERTY(EditDefaultsOnly, Category = "Player Movement | Mantle")
	float _mantleMaxAlignmentAngle = 55;

	UPROPERTY(EditDefaultsOnly, Category = "Player Movement | Mantle")
	TObjectPtr<UAnimMontage> _mantleAnimation;

	// Pushing
	UPROPERTY(EditDefaultsOnly, Category = "Pushing") 
	float MaxPushingSpeed = 50.f;
	UPROPERTY(EditDefaultsOnly, Category = "Pushing") 
	float ForwardSearchPushingLength = 85.f;
	UPROPERTY(EditDefaultsOnly, Category = "Pushing") 
	float PushingCapsuleRadius = 75.f;
	UPROPERTY(EditDefaultsOnly, Category = "Pushing") 
	FName MovableTag = "movable";
	EMovementMode _lastMode;

	TObjectPtr<AMoveableActor> _pushingActor;
	bool _isPushing;
};
