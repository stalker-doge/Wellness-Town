// Fill out your copyright notice in the Description page of Project Settings.


#include "MinimapComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Paper2D/Classes/PaperSpriteComponent.h"

// Sets default values for this component's properties
UMinimapComponent::UMinimapComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	_springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Minimap Spring Arm"));
	checkf(_springArm, TEXT("Missing Minimap SpringArmComponent"));

	_camera = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Minimap Camera"));
	checkf(_camera, TEXT("Missing Minimap Camera"));

	_sprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Sprite"));
	checkf(_sprite, TEXT("Missing Minimap PaperSprite"));
}

void UMinimapComponent::Init()
{
	_springArm->SetupAttachment(GetOwner()->GetRootComponent());
	_camera->SetupAttachment(_springArm);
	_sprite->SetupAttachment(GetOwner()->GetRootComponent());
}

// Called when the game starts
void UMinimapComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UMinimapComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

