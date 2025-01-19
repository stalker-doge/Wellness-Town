// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BugSpawner.generated.h"

class ABug;
class APlayerCharacter;

UCLASS()
class UE_WELLNESS_TOWN_API ABugSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABugSpawner();

	void Remove(ABug* bug);

	// Called every frame
	virtual void Tick(float DeltaTime) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	bool CheckIfPlayerInRange();

	void SpawnBug();
	void DespawnBugs();

private:	
	TObjectPtr<APlayerCharacter> _player;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ABug> _bugDefault;

	UPROPERTY(EditDefaultsOnly)
	float _despawnDelay;
	UPROPERTY(EditDefaultsOnly)
	float _activeRadiusFromPlayer;
	UPROPERTY(EditDefaultsOnly)
	float _spawnDelay;
	UPROPERTY(EditDefaultsOnly)
	float _timerOffset;
	TArray<TObjectPtr<ABug>> _spawnedBugs;
	float _despawnTimer;
	float _timer;

	bool _playerInRange;
};
