// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DestroyOnCollisionActor.generated.h"

class UStaticMeshComponent;
class USphereComponent;

UCLASS()
class UE_WELLNESS_TOWN_API ADestroyOnCollisionActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADestroyOnCollisionActor();

	//Sets the target that will trigger this to be destroyed
	UFUNCTION(BlueprintCallable)
	void SetCollisionTarget(AActor* target);

	UStaticMeshComponent* GetStaticMesh();

	UFUNCTION()
	void BeginInteractOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Called every frame
	virtual void Tick(float DeltaTime) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> _staticMesh;

	TWeakObjectPtr<AActor> _collisionTarget;
};
