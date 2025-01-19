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
	//--- MOVEMENT ----
	virtual void UpdateCharacterStateBeforeMovement(float DeltaSeconds) override;
	virtual void UpdateCharacterStateAfterMovement(float DeltaSeconds) override;
	void EnableMovement();
	void DisableMovement();

	//---- MANTLING ----
	//Checks if the player should be mantling
	void TryMantle();
	void OnMantleAnimFinished();

	//---- PUSHING ----
	//Checks if the player should be pushing
	void TryPushing();
	bool CanPush();
	//Updates physics for pushing
	void PhysPushing(float DeltaTime, int32 Iterations);
	AMoveableActor* GetPushedActor() const;

	//---- HELPER ----
	UFUNCTION(BlueprintPure)
	float GetSpeed() const;
	UFUNCTION(BlueprintPure)
	bool IsWalkingCustom() const;
	UFUNCTION(BlueprintPure)
	bool IsPushing() const;
	bool IsCustomMovementMode(const ECustomMovementMode InCustomMovementMode) const;
	float GetCapsuleRadius() const;
	float GetCapsuleHalfHeight() const;
	virtual float GetMaxSpeed() const override;

	//---- ANIMATION ----
	void InitAnimations();
	template<typename AnimNotify>
	AnimNotify* FindNotifyByClass(const TObjectPtr<UAnimSequenceBase> animation);

protected:
	virtual void InitializeComponent() override;
	virtual void PhysCustom(float deltaTime, int32 Iterations) override;

private:
	//---- MANTLING ----
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

	//---- PUSHING ----
	UPROPERTY(EditDefaultsOnly, Category = "Pushing") 
	float MaxPushingSpeed = 50.f;
	UPROPERTY(EditDefaultsOnly, Category = "Pushing") 
	float ForwardSearchPushingLength = 85.f;
	UPROPERTY(EditDefaultsOnly, Category = "Pushing") 
	float PushingCapsuleRadius = 75.f;
	UPROPERTY(EditDefaultsOnly, Category = "Pushing") 
	FName MovableTag = "movable";
	TObjectPtr<AMoveableActor> _pushedActor;
	bool _isPushing;

	UPROPERTY()
	TObjectPtr<APlayerCharacter> _player;

	int _rootMotionSourceID;
	TSharedPtr<FRootMotionSource_MoveToForce> _rootMotionSource;

	EMovementMode _lastMode;
};


