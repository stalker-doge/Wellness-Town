// Fill out your copyright notice in the Description page of Project Settings.


#include "DestroyOnCollisionActor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ADestroyOnCollisionActor::ADestroyOnCollisionActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_staticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	checkf(_staticMesh, TEXT("Static Mesh is an invalid value"));

	_staticMesh->SetSimulatePhysics(true);
	_staticMesh->SetupAttachment(RootComponent);
}

void ADestroyOnCollisionActor::SetCollisionTarget(TObjectPtr<AActor> target)
{
	_collisionTarget = target;
}

TObjectPtr<UStaticMeshComponent> ADestroyOnCollisionActor::GetStaticMesh()
{
	return _staticMesh;
}

void ADestroyOnCollisionActor::BeginInteractOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == _collisionTarget)
	{
		Destroy();
	}
}

// Called when the game starts or when spawned
void ADestroyOnCollisionActor::BeginPlay()
{
	Super::BeginPlay();
	
	_staticMesh->OnComponentBeginOverlap.AddDynamic(this, &ADestroyOnCollisionActor::BeginInteractOverlap);
}

// Called every frame
void ADestroyOnCollisionActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

