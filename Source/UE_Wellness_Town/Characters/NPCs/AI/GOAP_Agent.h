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
class AFishingRod;
class ABugNet;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_WELLNESS_TOWN_API UGOAP_Agent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGOAP_Agent();

	void Init();

	//---- GOAP ----
	TMap<FString, TObjectPtr<UGOAP_Belief>> GetBeliefs() const;
	TArray<TObjectPtr<UGOAP_Action>> GetActions() const;
	TArray<TObjectPtr<UGOAP_Goal>> GetGoals() const;

	//---- AI ----
	void SetPauseAgent(bool val);
	void TogglePauseAgent();
	void SetDestination(FVector destination);
	AActor* GetHome() const;
	AActor* GetWorkPlace() const;
	WorkType GetWorkType() const;
	void SetIsAtHome(bool val);
	void SetIsAtWorkPlace(bool val);
	bool IsAtHome() const;
	bool IsAtWorkPlace() const;
	bool IsSleepHours() const;
	bool IsWorkHours() const;

	//---- HELPERS ----
	USkeletalMeshComponent* GetMesh();
	ANPC_Base* GetNPC();
	FVector GetCurrentDestination() const;
	FVector GetActorLocation() const;
	FVector GetForwardVector() const;
	TSubclassOf<AFishingRod> GetFishingRodDefault() const;
	TSubclassOf<ABugNet> GetBugNetDefault() const;

	bool HasPath() const;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	//---- GOAP ----
	void SetupBeliefs();
	void SetupActions();	
	void SetupGoals();
	void CalculateActionPlan();
	void Reset();

private:
	//---- GOAP ----
	UPROPERTY()
	TMap<FString, TObjectPtr<UGOAP_Belief>> _beliefs;
	UPROPERTY()
	TArray<TObjectPtr<UGOAP_Action>> _actions;
	UPROPERTY()
	TArray<TObjectPtr<UGOAP_Goal>> _goals;
	UPROPERTY()
	TObjectPtr<UGOAP_Planner> _planner;
	UPROPERTY()
	TObjectPtr<UGOAP_Plan> _plan;
	UPROPERTY()
	TObjectPtr<UGOAP_Action> _currentAction;
	UPROPERTY()
	TObjectPtr<UGOAP_Goal> _currentGoal;
	UPROPERTY()
	TObjectPtr<UGOAP_Goal> _lastGoal;

	//---- AI ----
	UPROPERTY(EditDefaultsOnly, Category = "AI Data")
	TObjectPtr<AActor> _home;
	UPROPERTY(EditDefaultsOnly, Category = "AI Data")
	WorkType _workType;
	UPROPERTY(EditDefaultsOnly, Category = "AI Data")
	TObjectPtr<AActor> _workPlace;
	UPROPERTY(EditDefaultsOnly, Category = "AI Data")
	TSubclassOf<AFishingRod> _fishingRodDefault;
	UPROPERTY(EditDefaultsOnly, Category = "AI Data")
	TSubclassOf<ABugNet> _bugNetDefault;
	bool _isWorkHours;
	bool _isSleepHours;	
	bool _isAtHome;
	bool _isWorking;
	bool _isAtWorkPlace;

	TWeakObjectPtr<ANPC_Base> _owner;

	int _planFailCounter;
	bool _isPaused;

	float _timer;
};
