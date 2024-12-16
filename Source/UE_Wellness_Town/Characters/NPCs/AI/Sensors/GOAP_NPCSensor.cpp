// Fill out your copyright notice in the Description page of Project Settings.

#include "GOAP_NPCSensor.h"
#include "UE_Wellness_Town/Characters/NPCs/AI/GOAP_Agent.h"
#include "Components/SphereComponent.h"

// Sets default values for this component's properties
UGOAP_NPCSensor::UGOAP_NPCSensor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	_sensor = CreateDefaultSubobject<USphereComponent>(TEXT("Sensor"));
	checkf(_sensor, TEXT("Failed to create NPC Sensor Collider"));
}

void UGOAP_NPCSensor::Init(float radius, TObjectPtr<UGOAP_Agent> agent)
{
	_sensor->SetSphereRadius(radius);

	_agent = agent;
	_radius = radius;
}

void UGOAP_NPCSensor::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == this->GetOwner())
	{
		return;
	}

	if (_agent->_talkingTo != nullptr)
	{
		return;
	}

	UGOAP_Agent* agent = Cast<UGOAP_Agent>(OtherActor);

	if (agent == nullptr)
	{
		return;
	}

	if (agent->_talkingTo != nullptr)
	{
		return;
	}

	if (_agent->_canTalk == true && agent->_canTalk == true)
	{
		_agent->_talkingTo = agent;
		agent->_talkingTo = _agent;
	}
}

void UGOAP_NPCSensor::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

// Called when the game starts
void UGOAP_NPCSensor::BeginPlay()
{
	Super::BeginPlay();

	_sensor->OnComponentBeginOverlap.AddDynamic(this, &UGOAP_NPCSensor::BeginOverlap);
	_sensor->OnComponentEndOverlap.AddDynamic(this, &UGOAP_NPCSensor::EndOverlap);
}


// Called every frame
void UGOAP_NPCSensor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

