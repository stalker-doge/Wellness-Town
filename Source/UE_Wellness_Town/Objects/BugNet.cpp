// Fill out your copyright notice in the Description page of Project Settings.


#include "BugNet.h"
#include "Components/SphereComponent.h"
#include "UE_Wellness_Town/Characters/Animals/Bug.h"

ABugNet::ABugNet()
{
	_collisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Sphere"));
	checkf(_collisionSphere, TEXT("BugNet Missing Sphere Component"));

	_collisionSphere->SetCollisionProfileName(FName("OverlapAllDynamic"));
	_collisionSphere->SetupAttachment(_staticMesh);
	_collisionSphere->SetSimulatePhysics(false);

	_isCastable = true;
	_displaySpline = false;
}

bool ABugNet::ItemCast(AActor* player, USplineComponent* path, bool isPlayer)
{
	TArray<AActor*> overlappingActors;
	_collisionSphere->GetOverlappingActors(overlappingActors, ABug::StaticClass());

	for (AActor* actor : overlappingActors)
	{
		Cast<ABug>(actor)->RemoveFromSpawner();
		actor->Destroy();
	}

	return true;
}

void ABugNet::DisableCollision()
{
	_staticMesh->SetSimulatePhysics(false);
	_staticMesh->SetCollisionProfileName(FName("NoCollision"));
}

void ABugNet::BeginPlay()
{
	Super::BeginPlay();
}

void ABugNet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


