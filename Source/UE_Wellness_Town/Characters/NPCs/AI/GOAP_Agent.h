// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UE_Wellness_Town/Characters/NPCs/AI/WorkType.h"
#include "GOAP_Agent.generated.h"

class ANPC_Base;
class UGOAP_Belief;
class UGOAP_Goal;
class UGOAP_Planner;
class UGOAP_Action;
class UGOAP_Plan;
class UGOAP_NPCSensor;
class UTimeManager;
class AFishingRod;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_WELLNESS_TOWN_API UGOAP_Agent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGOAP_Agent();

	void Init();
	void SetupBeliefs();
	void SetupGoals();
	void SetupActions();

	AActor* GetHome();
	void SetIsAtHome(bool val);
	bool IsAtHome();

	AActor* GetWorkPlace();
	WorkType GetWorkType();
	void SetIsAtWorkPlace(bool val);
	bool IsAtWorkPlace();

	TSubclassOf<AFishingRod> GetFishingRodDefault();

	void Reset();
	UFUNCTION(BlueprintCallable)
	void SetPauseAgent(bool val);
	UFUNCTION(BlueprintCallable)
	void TogglePauseAgent();

	void CalculateActionPlan();

	void SetDestination(FVector destination);
	void SetActorLookAt(AActor* target);

	FVector GetActorLocation();
	FVector GetForwardVector();

	USkeletalMeshComponent* GetMesh();

	bool HasPath();


	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:

	UPROPERTY(EditAnywhere)
	TObjectPtr<AActor> _home;

	UPROPERTY(EditAnywhere)
	WorkType _workType;
	UPROPERTY(EditAnywhere)
	TObjectPtr<AActor> _workPlace;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AFishingRod> _fishingRodDefault;

	TObjectPtr<ANPC_Base> _owner;
	TObjectPtr<UGOAP_NPCSensor> _npcSensor;

	UPROPERTY()
	TMap<FString, TObjectPtr<UGOAP_Belief>> _beliefs;

	TObjectPtr<UGOAP_Agent> _talkingTo;
	bool _canTalk;

	UPROPERTY()
	TObjectPtr<UGOAP_Goal> _currentGoal;
	UPROPERTY()
	TObjectPtr<UGOAP_Goal> _lastGoal;
	UPROPERTY()
	TObjectPtr<UGOAP_Plan> _plan;
	UPROPERTY()
	TObjectPtr<UGOAP_Action> _currentAction;

	UPROPERTY()
	TArray<TObjectPtr<UGOAP_Action>> _actions;
	UPROPERTY()
	TArray<TObjectPtr<UGOAP_Goal>> _goals;

	UPROPERTY()
	TObjectPtr<UGOAP_Planner> _planner;

	TObjectPtr<UTimeManager> _timeManager;
	bool _isWorkHours;
	bool _isSleepHours;

	bool _isAtHome;

	bool _isWorking;
	bool _isAtWorkPlace;

	int _planFailCounter;
	bool _isPaused;
};
