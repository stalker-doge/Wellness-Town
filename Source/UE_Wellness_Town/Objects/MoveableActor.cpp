// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveableActor.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

// Sets default values
AMoveableActor::AMoveableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	_meshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	_meshComponent->SetSimulatePhysics(true);
	_meshComponent->SetLinearDamping(5.f);
	SetRootComponent(_meshComponent);

	_physicsComponent = CreateDefaultSubobject<UPhysicsConstraintComponent>("PhysicsComponent");
	_physicsComponent->SetLinearXLimit(LCM_Free, 0.f);
	_physicsComponent->SetAngularSwing1Limit(ACM_Locked, 0.f);
	_physicsComponent->SetAngularSwing2Limit(ACM_Locked, 0.f);
	_physicsComponent->SetAngularTwistLimit(ACM_Locked, 0.f);

	FConstrainComponentPropName ComponentPropName;
	ComponentPropName.ComponentName = "MeshComponent";
	_physicsComponent->ComponentName1 = ComponentPropName;
	_physicsComponent->SetupAttachment(_meshComponent);

	Tags.Add("movable");
}

void AMoveableActor::AddForce(TObjectPtr<AActor> target)
{
	FVector direction = GetActorLocation() - target->GetActorLocation();

	_meshComponent->AddImpulse(-direction * 600);
}

