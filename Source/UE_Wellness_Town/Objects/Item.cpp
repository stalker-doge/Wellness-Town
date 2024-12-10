// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "Components/StaticMeshComponent.h"
#include "UE_Wellness_Town/Characters/Player/PlayerCharacter.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_staticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	checkf(_staticMesh, TEXT("Static Mesh is an invalid value"));

	_staticMesh->SetCollisionProfileName(FName("PhysicsInteractable"));
	_staticMesh->SetSimulatePhysics(true);
	_staticMesh->SetupAttachment(RootComponent);
}

TObjectPtr<UStaticMeshComponent> AItem::GetStaticMesh()
{
	return _staticMesh;
}

void AItem::DisableCollision()
{
	_staticMesh->SetSimulatePhysics(false);
	_staticMesh->SetCollisionProfileName(FName("NoCollision"));
}

void AItem::EnableCollision()
{
	_staticMesh->SetCollisionProfileName(FName("PhysicsInteractable"));
	_staticMesh->SetSimulatePhysics(true);
}

void AItem::Interact(APlayerCharacter* player)
{
	player->PickUp(this);
}

bool AItem::ItemCast(AActor* player, USplineComponent* path)
{
	if (path == nullptr)
	{
		return false;
	}

	return true;
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

