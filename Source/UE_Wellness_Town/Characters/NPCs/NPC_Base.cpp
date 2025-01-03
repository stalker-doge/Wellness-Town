// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_Base.h"
#include "UE_Wellness_Town/Characters/NPCs/NPC_Data.h"
#include "UE_Wellness_Town/Dialogue/DialogueSystem.h"
#include "UE_Wellness_Town/Characters/NPCs/AI/GOAP_Agent.h"
#include "UE_Wellness_Town/Characters/Player/PlayerCharacter.h"
#include "UE_Wellness_Town/Objects/Item.h"
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SplineComponent.h"

// Sets default values
ANPC_Base::ANPC_Base()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_data = CreateDefaultSubobject<UNPC_Data>(TEXT("NPC Info"));
	checkf(_data, TEXT("NPC is missing NPC_Data component"));

	_splineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Spline Component"));
	checkf(_splineComponent, TEXT("Spline Component is an invalid value"));

	_agent = CreateDefaultSubobject<UGOAP_Agent>(TEXT("AI Agent"));
	checkf(_agent, TEXT("NPC is missing GOAP_Agent component"));

	_agent->Init();
}

void ANPC_Base::UseItem(USplineComponent* path)
{
	if (_heldObject == nullptr)
	{
		return;
	}

	_heldObject->ItemCast(this, path, false);
}

void ANPC_Base::SetDestination(FVector destination)
{
	_currentDestination = destination;

	FAIMoveRequest moveTo;
	moveTo.SetAcceptanceRadius(10);
	moveTo.SetGoalLocation(destination);

	_controller->MoveTo(moveTo);
}

FVector ANPC_Base::GetCurrentDestination()
{
	return _currentDestination;
}

void ANPC_Base::SetPauseMovement(bool val)
{
	if (val == true)
	{
		_controller->PauseMove(FAIRequestID::CurrentRequest);
		return;
	}

	_controller->ResumeMove(FAIRequestID::CurrentRequest);
}

bool ANPC_Base::HasPath()
{
	return _controller->IsFollowingAPath();
}

void ANPC_Base::SetPauseAI(bool val)
{
	_agent->SetPauseAgent(val);
}


void ANPC_Base::PickUp(AItem* item)
{
	if (_heldObject != nullptr)
	{
		DropItem();
	}

	_heldObject = item;
	_heldObject->DisableCollision();
	item->SetActorLocation(GetMesh()->GetSocketLocation("HoldSocket"));
	item->K2_AttachToComponent(GetMesh(), "HoldSocket", EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, true);

	if (IsInPlayerRange() == false)
	{
		_heldObject->GetStaticMesh()->ToggleVisibility(false);
	}
}

void ANPC_Base::DropItem()
{
	if (_heldObject == nullptr)
	{
		return;
	}

	_heldObject->EnableCollision();
	_heldObject->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	_heldObject = nullptr;
}

USplineComponent* ANPC_Base::GetSplineComponent()
{
	return _splineComponent;
}

UNPC_Data* ANPC_Base::GetNPCData()
{
	return _data.Get();
}

// Current dialogue was just to check for errors with NPC_Data access. Remove/change if needed for the full Dialogue implementation
void ANPC_Base::Interact(APlayerCharacter* player)
{
	_agent->TogglePauseAgent();

	/*if (_dialogueDefault == nullptr)
	{
		return;
	}

	if (_dialogueComponent == nullptr)
	{
		_dialogueComponent = Cast<UDialogueSystem>(AddComponentByClass(_dialogueDefault, false, GetTransform(), true));
	}

	_dialogueComponent->StartDialogue();*/
	GEngine->AddOnScreenDebugMessage(4, 10, FColor::Green, FString::Printf(TEXT("Interacted With: %s"), *_data->GetNPCName()));
}

// Called when the game starts or when spawned
void ANPC_Base::BeginPlay()
{
	Super::BeginPlay();
	
	_controller = Cast<AAIController>(GetController());
	_player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

bool ANPC_Base::IsInPlayerRange()
{
	if (_player == nullptr)
	{
		return false;
	}

	float distance = (GetActorLocation() - _player->GetActorLocation()).Length();

	if (distance > _hideDistanceFromPlayer)
	{
		if (GetMesh()->IsVisible() == true)
		{
			if (_heldObject != nullptr)
			{
				_heldObject->GetStaticMesh()->ToggleVisibility(false);
			}

			GetMesh()->ToggleVisibility(false);
		}

		return false;
	}

	if (GetMesh()->IsVisible() == false && _agent->IsAtHome() == false)
	{
		if (_heldObject != nullptr)
		{
			_heldObject->GetStaticMesh()->ToggleVisibility(true);
		}

		GetMesh()->ToggleVisibility(true);
	}

	return true;
}

// Called every frame
void ANPC_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	_timer += DeltaTime;

	if (_timer > 1)
	{
		IsInPlayerRange();
		_timer = 0;
	}
}

// Called to bind functionality to input
void ANPC_Base::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

