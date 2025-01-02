// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GOAP_BugSensor.generated.h"

class USphereComponent;
class ABug;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_WELLNESS_TOWN_API UGOAP_BugSensor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGOAP_BugSensor();

	void Init(float radius, AActor* owner);
	void SetIsActive(bool val);
	ABug* GetClosestBug();

	void UpdateClosestBug();

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


private:	
	UPROPERTY(EditAnywhere)
	TObjectPtr<USphereComponent> _sensor;

	UPROPERTY()
	TObjectPtr<AActor> _owner;
	UPROPERTY()
	TObjectPtr<ABug> _closestBug;

	float _radius;
	bool _isActive;
};
