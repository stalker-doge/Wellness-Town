// Fill out your copyright notice in the Description page of Project Settings.


#include "Bug.h"
#include "Components/StaticMeshComponent.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "UE_Wellness_Town/Characters/Animals/BugSpawner.h"

// Sets default values
ABug::ABug()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_bugMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	checkf(_bugMesh, TEXT("Bug missing Static Mesh"));
	_bugMesh->SetupAttachment(RootComponent);
	_bugMesh->SetCollisionProfileName(FName("NoCollision"));
}

void ABug::Init(ABugSpawner* spawner)
{
	_spawner = spawner;
}

void ABug::RemoveFromSpawner()
{
	if (_spawner.IsValid() == true)
	{
		_spawner->Remove(this);
	}
}

// Called when the game starts or when spawned
void ABug::BeginPlay()
{
	Super::BeginPlay();
	
	_navSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	_controller = Cast<AAIController>(GetController());
}

// Called every frame
void ABug::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (_controller->IsFollowingAPath() == true)
	{
		return;
	}

	FNavLocation location;
	_navSystem->GetRandomPointInNavigableRadius(GetActorLocation(), 500, location);

	FAIMoveRequest moveTo;
	moveTo.SetAcceptanceRadius(10);
	moveTo.SetGoalLocation(location);

	_controller->MoveTo(moveTo);
}

// Called to bind functionality to input
void ABug::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

