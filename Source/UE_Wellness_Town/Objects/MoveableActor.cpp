// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveableActor.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "UE_Wellness_Town/Characters/Player/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/AnimGraphRuntime/Public/KismetAnimationLibrary.h"

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

	FConstrainComponentPropName componentPropName;
	componentPropName.ComponentName = "MeshComponent";
	_physicsComponent->ComponentName1 = componentPropName;
	_physicsComponent->SetupAttachment(_meshComponent);

	Tags.Add("movable");
}

void AMoveableActor::AddForce(APlayerCharacter* target)
{
	//Gets the direction the player is travelling relative to this object
	float playerDir = UKismetAnimationLibrary::CalculateDirection(target->GetVelocity(), target->GetActorRotation());

	//Gets the direction from the player to this object
	FVector direction = GetActorLocation() - target->GetActorLocation();
	float dirX = direction.X;
	float dirY = direction.Y;

	direction.Normalize();

	float dot = FMath::Abs(GetActorForwardVector().Dot(direction));

	float speed = target->GetMaxSpeed();

	//Checks if the player is to the side or ahead/behind the object
	if (dot > 0.5)
	{
		//Checks if the player is on the left or right side, then applies force
		if (dirX < -180 && playerDir < -100)
		{
			_meshComponent->AddImpulse(target->GetVelocity() * speed);
			return;
		}

		if (dirX > 180 && playerDir > 100)
		{
			_meshComponent->AddImpulse(target->GetVelocity() * speed);
			return;
		}
	}
	else
	{
		//Checks if the player is on the front or back side, then applies force
		if (dirY < -180 && playerDir > 100)
		{
			_meshComponent->AddImpulse(target->GetVelocity() * speed);
			return;
		}

		if (dirY > 180 && playerDir < -100)
		{
			_meshComponent->AddImpulse(target->GetVelocity() * speed);
			return;
		}
	}
}

