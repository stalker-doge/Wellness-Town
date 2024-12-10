// Fill out your copyright notice in the Description page of Project Settings.


#include "FishingRod.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SplineComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UE_Wellness_Town/Characters/Player/PlayerCharacter.h"
#include "UE_Wellness_Town/Objects/DestroyOnCollisionActor.h"

// Sets default values
AFishingRod::AFishingRod()
{
	_isCastable = true;
}

void AFishingRod::InitFishing()
{
	_isFishing = true;
}

void AFishingRod::CatchLoot()
{
	_isFishing = false;
	TSubclassOf<ADestroyOnCollisionActor> toSpawn = _loot[0];

	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	TObjectPtr<ADestroyOnCollisionActor> loot = GetWorld()->SpawnActor<ADestroyOnCollisionActor>(*toSpawn, _end + FVector(0, 50, 0), FRotator::ZeroRotator, spawnParams);
	loot->SetCollisionTarget(_player);

	FVector dir = _player->GetActorLocation() - loot->GetActorLocation();
	FRotator angle = dir.Rotation();
	loot->SetActorRotation(angle);

	FVector unitDirection = loot->GetActorForwardVector() + loot->GetActorUpVector();
	unitDirection.Normalize();

	UStaticMeshComponent* staticMeshComponent = loot->GetStaticMesh();
	staticMeshComponent->SetSimulatePhysics(true);
	staticMeshComponent->SetPhysicsLinearVelocity(unitDirection * 1000);

	_player->EnableMovement();
	_player = nullptr;
}

bool AFishingRod::ItemCast(AActor* player, USplineComponent* path)
{
	AItem::ItemCast(player, path);

	_player = Cast<APlayerCharacter>(player);

	TArray<AActor*> hitData;
	TArray<AActor*> toIgnore;
	toIgnore.Add(player);
	toIgnore.Add(GetOwner());

	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
	objectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	objectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));

	for (int i = 0; i < path->GetNumberOfSplinePoints(); i++)
	{
		if (i == 0)
		{
			_start = path->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World);
		}

		if (UKismetSystemLibrary::SphereOverlapActors(this, path->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local), 0.5f, objectTypes, nullptr, toIgnore, hitData))
		{
			for (AActor* data : hitData)
			{
				if (data->Tags.Contains("Water") == false)
				{
					return false;
				}
				else
				{
					_end = path->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local);
					_player->DisableMovement();
					InitFishing();
					return true;
				}
			}
		}
	}

	return false;
}

// Called when the game starts or when spawned
void AFishingRod::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFishingRod::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (_isFishing == false)
	{
		return;
	}

	_timer += DeltaTime;

	if (_timer >= 3)
	{
		CatchLoot();
		_timer = 0;
	}
}

