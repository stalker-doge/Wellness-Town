// Fill out your copyright notice in the Description page of Project Settings.


#include "BugSpawner.h"
#include "UE_Wellness_Town/Characters/Animals/Bug.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UE_Wellness_Town/Characters/Player/PlayerCharacter.h"

// Sets default values
ABugSpawner::ABugSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ABugSpawner::Remove(ABug* bug)
{
	if (_spawnedBugs.Contains(bug))
	{
		_spawnedBugs.Remove(bug);
	}
}

// Called when the game starts or when spawned
void ABugSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	_timer = _timerOffset;
	_player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

bool ABugSpawner::CheckIfPlayerInRange()
{
	if (_player == nullptr)
	{
		return false;
	}

	FVector dist = GetActorLocation() - _player->GetActorLocation();

	if (dist.Length() > _activeRadiusFromPlayer)
	{
		return false;
	}

	return true;
}

void ABugSpawner::SpawnBug()
{
	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	TObjectPtr<ABug> bug = GetWorld()->SpawnActor<ABug>(_bugDefault, GetActorLocation(), FRotator::ZeroRotator, spawnParams);
	_spawnedBugs.Add(bug);
	bug->Init(this);
}

void ABugSpawner::DespawnBugs()
{
	for (ABug* bug : _spawnedBugs)
	{
		bug->Destroy();
	}

	_spawnedBugs.Empty();
}

// Called every frame
void ABugSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	_playerInRange = CheckIfPlayerInRange();

	if (_playerInRange == true)
	{
		_timer += DeltaTime;
		_despawnTimer = 0;
	}
	else
	{
		_timer = 0;

		if (_spawnedBugs.Num() > 0)
		{
			_despawnTimer += DeltaTime;
		}
	}

	if (_timer > _spawnDelay)
	{
		SpawnBug();
		_timer = 0;
	}

	if (_despawnTimer > _despawnDelay)
	{
		DespawnBugs();
		_despawnTimer = 0;
	}
}

