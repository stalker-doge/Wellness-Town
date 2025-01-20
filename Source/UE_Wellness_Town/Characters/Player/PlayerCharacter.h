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
class UMinimapComponent;
class AItem;
class UPlayerHUD;

UCLASS()
class UE_WELLNESS_TOWN_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	//---- MOVEMENT ----
	UFUNCTION(BlueprintCallable)
	void EnableMovement();
	UFUNCTION(BlueprintCallable)
	void DisableMovement();

	//---- ITEM ----
	void PickUp(AItem* actor);
	void DropHeldItem();
	void CastHeldItem();
	void ThrowHeldItem();

	//---- INTERACTION ----
	void Interact();
	void AltInteract();
	UFUNCTION()
	void BeginInteractOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void EndInteractOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//---- HELPER ----
	UFUNCTION(BlueprintCallable)
	AActor* GetInteractTarget();
	UFUNCTION(BlueprintCallable)
	float GetMaxSpeed() const;
	UFUNCTION(BlueprintCallable)
	UCameraComponent* GetCamera();
	UFUNCTION(BlueprintCallable)
	USpringArmComponent* GetSpringArm();
	UFUNCTION(BlueprintCallable)
	USphereComponent* GetInteractCollider();
	void IsReadyToCast();
	void IsReadyToThrow();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//---- SPLINE HANDLING ----
	//Creates the visual for the spline if needed
	void DrawTrajectory();
	void CreateSpline(FVector start, FVector unitDirection, float strength, int simTime, bool toDebug);
	void ClearSpline();
private:
	///---- STATS ----
	UPROPERTY(EditAnywhere, Category = "Player Stats")
	float _throwStrength;

	//---- COMPONENTS ----
	UPROPERTY(EditAnywhere, Category = "Camera")
	TObjectPtr<UCameraComponent> _camera;
	UPROPERTY(EditAnywhere, Category = "Camera")
	TObjectPtr<USpringArmComponent> _springArmComponent;
	UPROPERTY(EditDefaultsOnly, Category = "Minimap")
	TObjectPtr<UMinimapComponent> _minimap;
	UPROPERTY(EditAnywhere, Category = "Interaction")
	TObjectPtr<USphereComponent> _interactCollider;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UPlayerMovementComponent> _movementComponent;

	//---- SPLINE ----
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> _splineMesh;
	TArray<TObjectPtr<AActor>> _splineMeshes;
	TObjectPtr<USplineComponent> _splineComponent;

	//---- UI ----
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UPlayerHUD> _hudDefault;
	TObjectPtr<UPlayerHUD> _playerHUD;

	TWeakObjectPtr<AItem> _heldObject;
	TWeakObjectPtr<AActor> _currentInteractTarget;

	FVector _lastLocation;
	FRotator _lastRotation;

	bool _drawTrajectory;
	bool _isAltInteracting;
	float _timer;
};
