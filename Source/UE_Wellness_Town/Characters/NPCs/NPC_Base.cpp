// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_Base.h"
#include "UE_Wellness_Town/Characters/NPCs/NPC_Data.h"
#include "UE_Wellness_Town/Dialogue/DialogueSystem.h"
#include "UE_Wellness_Town/Characters/NPCs/AI/GOAP_Agent.h"
#include "UE_Wellness_Town/Characters/Player/PlayerCharacter.h"
#include "UE_Wellness_Town/Objects/Item.h"
#include "UE_Wellness_Town/Characters/NPCs/Mood.h"
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SplineComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

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

	_niagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara Component"));
	_niagara->SetupAttachment(GetMesh(), FName("NiagaraSocket"));
}

void ANPC_Base::ToggleVisibility(bool isVisible)
{
	if (_heldObject != nullptr)
	{
		_heldObject->GetStaticMesh()->ToggleVisibility(isVisible);
	}

	GetMesh()->ToggleVisibility(isVisible);
	_niagara->SetActive(isVisible);
}

#pragma region "AI"
void ANPC_Base::SetDestination(FVector destination)
{
	_currentDestination = destination;

	FAIMoveRequest moveTo;
	moveTo.SetAcceptanceRadius(10);
	moveTo.SetGoalLocation(destination);

	_controller->MoveTo(moveTo);
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

void ANPC_Base::SetPauseAI(bool val)
{
	_agent->SetPauseAgent(val);
}

void ANPC_Base::SetMood(Mood mood)
{
	_mood = mood;

	if (_niagaraSystems.Contains(mood) == false)
	{
		_niagara->Deactivate();
		return;
	}

	_niagara->SetAsset(_niagaraSystems[_mood]);
	_niagara->Activate();
}
#pragma endregion

#pragma region "ITEM"
void ANPC_Base::UseItem(USplineComponent* path)
{
	if (_heldObject.IsValid() == false)
	{
		return;
	}

	_heldObject->ItemCast(this, path, false);
}

void ANPC_Base::PickUp(AItem* item)
{
	if (_heldObject.IsValid() == true)
	{
		DropItem();
	}

	_heldObject = item;
	_heldObject->DisableCollision();
	item->SetActorLocation(GetMesh()->GetSocketLocation("HoldSocket"));
	item->K2_AttachToComponent(GetMesh(), "HoldSocket", EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, true);

	if (IsInPlayerRange() == false)
	{
		ToggleVisibility(false);
	}
}

void ANPC_Base::DropItem()
{
	if (_heldObject.IsValid() == false)
	{
		return;
	}

	_heldObject->EnableCollision();
	_heldObject->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	_heldObject = nullptr;
}
#pragma endregion

#pragma region "HELPER"
UNPC_Data* ANPC_Base::GetNPCData()
{
	return _data.Get();
}

FVector ANPC_Base::GetCurrentDestination() const
{
	return _currentDestination;
}

USplineComponent* ANPC_Base::GetSplineComponent()
{
	return _splineComponent;
}

bool ANPC_Base::HasPath() const
{
	return _controller->IsFollowingAPath();
}

bool ANPC_Base::IsInPlayerRange()
{
	if (_player.IsValid() == false)
	{
		return false;
	}

	float distance = (GetActorLocation() - _player->GetActorLocation()).Length();

	if (distance > _hideDistanceFromPlayer)
	{
		if (GetMesh()->IsVisible() == true)
		{
			ToggleVisibility(false);
		}

		return false;
	}

	if (GetMesh()->IsVisible() == false && _agent->IsAtHome() == false)
	{
		ToggleVisibility(true);
	}

	return true;
}
#pragma endregion

void ANPC_Base::Interact(APlayerCharacter* player)
{
	if (_data->GetNPCName() == "Unknown")
	{
		_data->LearnName();
	}

	GEngine->AddOnScreenDebugMessage(4, 10, FColor::Green, FString::Printf(TEXT("Interacted With: %s"), *_data->GetNPCName()));
}

// Called when the game starts or when spawned
void ANPC_Base::BeginPlay()
{
	Super::BeginPlay();
	
	_controller = Cast<AAIController>(GetController());
	_player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	_mood = _startingMood;
	SetMood(_mood);
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

