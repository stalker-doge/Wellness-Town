// Fill out your copyright notice in the Description page of Project Settings.


#include "GOAP_BugSensor.h"
#include "Components/SphereComponent.h"
#include "UE_Wellness_Town/Characters/Animals/Bug.h"

// Sets default values for this component's properties
UGOAP_BugSensor::UGOAP_BugSensor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UGOAP_BugSensor::Init(float radius, AActor* owner)
{
	_owner = owner;
	_radius = radius;

	_sensor = NewObject<USphereComponent>(_owner->GetOwner(), TEXT("Bug Sensor SphereCollider"));
	checkf(_sensor, TEXT("Failure to Create Bug Sensor SphereCollider"));

	_sensor->RegisterComponent();

	_sensor->AttachToComponent(_owner->GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	_sensor->SetCollisionProfileName("BugSensor");

	_sensor->OnComponentBeginOverlap.AddDynamic(this, &UGOAP_BugSensor::BeginOverlap);
	_sensor->OnComponentEndOverlap.AddDynamic(this, &UGOAP_BugSensor::EndOverlap);

	_sensor->SetSphereRadius(radius);
}

void UGOAP_BugSensor::SetIsActive(bool val)
{
	_sensor->SetGenerateOverlapEvents(val);

	if (val == true)
	{
		UpdateClosestBug();
	}
}

ABug* UGOAP_BugSensor::GetClosestBug()
{
	return _closestBug.Get();
}

void UGOAP_BugSensor::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == _owner)
	{
		return;
	}

	ABug* bug = Cast<ABug>(OtherActor);

	if (bug == nullptr)
	{
		return;
	}

	UpdateClosestBug();
}

void UGOAP_BugSensor::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == _owner)
	{
		return;
	}

	ABug* bug = Cast<ABug>(OtherActor);

	if (bug == nullptr)
	{
		return;
	}

	UpdateClosestBug();
}


// Called when the game starts
void UGOAP_BugSensor::BeginPlay()
{
	Super::BeginPlay();
}

void UGOAP_BugSensor::UpdateClosestBug()
{
	TArray<AActor*> overlappingActors;
	_sensor->GetOverlappingActors(overlappingActors, ABug::StaticClass());

	if (overlappingActors.Num() == 0)
	{
		_closestBug = nullptr;
		return;
	}

	float closestDistance = 99999999999;

	for (AActor* actor : overlappingActors)
	{
		FVector distance = _owner->GetActorLocation() - actor->GetActorLocation();

		if (distance.Length() < closestDistance)
		{
			_closestBug = Cast<ABug>(actor);
			closestDistance = distance.Length();
		}
	}
}


// Called every frame
void UGOAP_BugSensor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

