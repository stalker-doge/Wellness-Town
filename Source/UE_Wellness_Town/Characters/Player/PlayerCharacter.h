// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USphereComponent;
class UPlayerMovementComponent;
class USplineComponent;
class USplineMeshComponent;
class AItem;

UCLASS()
class UE_WELLNESS_TOWN_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	UFUNCTION(BlueprintCallable)
	UCameraComponent* GetCamera();

	void PickUp(AItem* actor);
	void DropHeldItem();
	void CastHeldItem();
	void IsReadyToCast();
	void ThrowHeldItem();
	void IsReadyToThrow();

	void Interact();
	void AltInteract();

	UFUNCTION(BlueprintCallable)
	AActor* GetInteractTarget();

	UFUNCTION(BlueprintCallable)
	float GetMaxSpeed();

	UFUNCTION(BlueprintCallable)
	void EnableMovement();
	UFUNCTION(BlueprintCallable)
	void DisableMovement();

	UFUNCTION()
	void BeginInteractOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void EndInteractOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact Collider")
	TObjectPtr<USphereComponent> _interactCollider;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool IsAltInteracting;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Below functions manages throwing trajectory
	//Creates the visual for the spline if needed
	void DrawTrajectory();
	void CreateSpline(FVector start, FVector unitDirection, float strength, int simTime, bool toDebug);
	void ClearSpline();
private:

	UPROPERTY(EditAnywhere)
	TObjectPtr<UCameraComponent> _camera;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USpringArmComponent> _springArmComponent;

	UPROPERTY()
	TObjectPtr<UPlayerMovementComponent> _movementComponent;

	TObjectPtr<AActor> _currentInteractTarget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> _splineMesh;

	TObjectPtr<USplineComponent> _splineComponent;
	TArray<TObjectPtr<AActor>> _splineMeshes;

	UPROPERTY(EditAnywhere)
	float _throwStrength;

	TObjectPtr<AItem> _heldObject;
	float _timer;

	FVector _lastLocation;
	FRotator _lastRotation;

	bool _drawTrajectory;
};
