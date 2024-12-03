// Fill out your copyright notice in the Description page of Project Settings.


#include "TestInteractable.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ATestInteractable::ATestInteractable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	_staticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	checkf(_staticMesh, TEXT("Static Mesh is an invalid value"));

	_staticMesh->SetCollisionProfileName(FName("Interactable"));
	_staticMesh->SetupAttachment(RootComponent);
}

void ATestInteractable::Interact(TObjectPtr<APlayerCharacter> player)
{
	GEngine->AddOnScreenDebugMessage(3, 10, FColor::Green, FString::Printf(TEXT("Interacted With: %s"), *GetName()));
}

// Called when the game starts or when spawned
void ATestInteractable::BeginPlay()
{
	Super::BeginPlay();
	
}

