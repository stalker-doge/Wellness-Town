// Fill out your copyright notice in the Description page of Project Settings.


#include "Journal.h"
#include "UE_Wellness_Town/UI/JournalWidget.h"
#include "Blueprint/UserWidget.h"
#include "UE_Wellness_Town/Characters/Player/PlayerCharacter.h"
#include "UE_Wellness_Town/GameModes/WT_GameMode.h"
#include "UE_Wellness_Town/Characters/NPCs/NPC_Manager.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AJournal::AJournal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_staticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	checkf(_staticMesh, TEXT("Static Mesh is invalid value"));

	_staticMesh->SetCollisionProfileName(FName("Interactable"));
	_staticMesh->SetupAttachment(RootComponent);
}

void AJournal::Init(UNPC_Manager* npcmanager)
{
	_npcManager = npcmanager;

	//Creates the journal widget and hides it
	_journalWidget = CreateWidget<UJournalWidget>(GetWorld()->GetFirstPlayerController<APlayerController>(), _journalWidgetDefault);
	_journalWidget->RemoveFromViewport();

	//Assigns the maxpage based on the total NPC_Data in the world. If the total is odd, the max page is incremented by one (as one side of the page will be needed)
	_maxPage = _npcManager->GetNPCData().Num();

	if (_maxPage % 2 != 0)
	{
		_maxPage += 1;
	}
}

//Displays the Journal Widget and changes the player input mode to UI only.
void AJournal::Interact(APlayerCharacter* player)
{
	_journalWidget->AddToViewport();

	FInputModeUIOnly inputMode;
	inputMode.SetWidgetToFocus(_journalWidget->TakeWidget());
	GetWorld()->GetFirstPlayerController<APlayerController>()->SetInputMode(inputMode);
	GetWorld()->GetFirstPlayerController<APlayerController>()->SetShowMouseCursor(true);

	_journalWidget->Init(_npcManager->GetNPCData());
}

// Called when the game starts or when spawned
void AJournal::BeginPlay()
{
	Super::BeginPlay();

	checkf(_journalWidgetDefault, TEXT("Missing Journal Widget Reference"));
}

// Called every frame
void AJournal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

