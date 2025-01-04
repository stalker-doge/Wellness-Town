// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MinimapComponent.generated.h"

class USpringArmComponent;
class USceneCaptureComponent2D;
class UPaperSpriteComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_WELLNESS_TOWN_API UMinimapComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMinimapComponent();

	void Init();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<USpringArmComponent> _springArm;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneCaptureComponent2D> _camera;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UPaperSpriteComponent> _sprite;
};
