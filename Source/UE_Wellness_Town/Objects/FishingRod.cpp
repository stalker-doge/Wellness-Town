// Fill out your copyright notice in the Description page of Project Settings.


#include "FishingRod.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SplineComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UE_Wellness_Town/Characters/Player/PlayerCharacter.h"
#include "UE_Wellness_Town/Characters/NPCs/NPC_Base.h"
#include "UE_Wellness_Town/Objects/DestroyOnCollisionActor.h"

// Sets default values
AFishingRod::AFishingRod()
{
	_isCastable = true;
	_displaySpline = true;
}

void AFishingRod::InitFishing()
{
	_isFishing = true;
}

void AFishingRod::CatchLoot()
{
	//Generates which loot will be spawned, will be changed to a random value when more is added
	TSubclassOf<ADestroyOnCollisionActor> toSpawn = _loot[0];

	//Spawns generated loot
	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	TObjectPtr<ADestroyOnCollisionActor> loot = GetWorld()->SpawnActor<ADestroyOnCollisionActor>(*toSpawn, _end, FRotator::ZeroRotator, spawnParams);
	loot->SetCollisionTarget(_owner.Get());

	//Faces the loot towards the player
	FVector dir = _owner->GetActorLocation() - loot->GetActorLocation();
	FRotator angle = dir.Rotation();
	loot->SetActorRotation(angle);

	//Apply physics to the loot object to make it "fly" towards the player
	FVector unitDirection = loot->GetActorForwardVector() + loot->GetActorUpVector();
	unitDirection.Normalize();

	UStaticMeshComponent* staticMeshComponent = loot->GetStaticMesh();
	staticMeshComponent->SetSimulatePhysics(true);
	staticMeshComponent->SetPhysicsLinearVelocity(unitDirection * 1000);

	_lootSpawned = true;
}

bool AFishingRod::ItemCast(AActor* player, USplineComponent* path, bool isPlayer)
{
	_owner = player;
	_isPlayer = isPlayer;

	AItem::ItemCast(player, path, isPlayer);

	TArray<AActor*> hitData;
	TArray<AActor*> toIgnore;
	toIgnore.Add(player);
	toIgnore.Add(GetOwner());

	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
	objectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	objectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));

	//Does a collision check on each point in the spline provided, if a point is in contact with an actor that isn't "Water", the fishing line would be blocked. 
	//Returns true if the path collides with water without being blocked
	for (int i = 0; i < path->GetNumberOfSplinePoints(); i++)
	{
		if (UKismetSystemLibrary::SphereOverlapActors(this, path->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local), 0.5f, objectTypes, nullptr, toIgnore, hitData))
		{
			for (AActor* data : hitData)
			{
				if (data->Tags.Contains("Water") == false)
				{
					return false;
				}

				_end = path->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local);

				if (_isPlayer == true)
				{
					Cast<APlayerCharacter>(_owner)->DisableMovement();
				}
				else
				{
					Cast<ANPC_Base>(_owner)->SetPauseAI(true);
				}

				InitFishing();
				return true;
			}
		}
	}

	return false;
}

bool AFishingRod::IsFishing()
{
	return _isFishing;
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
		if (_lootSpawned == false)
		{
			CatchLoot();
		}
	}

	if (_timer >= 5)
	{
		if (_isPlayer == true)
		{
			Cast<APlayerCharacter>(_owner)->EnableMovement();
		}
		else
		{
			Cast<ANPC_Base>(_owner)->SetPauseAI(false);
		}

		_owner = nullptr;
		_lootSpawned = false;
		_isFishing = false;
		_timer = 0;
	}
}

