// Fill out your copyright notice in the Description page of Project Settings.


#include "WT_GameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/Actor.h"
#include "UE_Wellness_Town/Characters/NPCs/NPC_Manager.h"
#include "UE_Wellness_Town/Utility/TimeManager.h"
#include "UE_Wellness_Town/Objects/Journal.h"
#include "Kismet/GameplayStatics.h"

AWT_GameMode::AWT_GameMode()
{
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;
}

UNPC_Manager* AWT_GameMode::GetNPCManager()
{
	return _npcManager;
}

UTimeManager* AWT_GameMode::GetTimeManager()
{
	return _timeManager;
}

void AWT_GameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	_timeManager->Update(DeltaTime);
}

void AWT_GameMode::BeginPlay()
{
	Super::BeginPlay();

	//Creates NPC Manager
	_npcManager = NewObject<UNPC_Manager>();
	checkf(_npcManager, TEXT("NPC_Manager is invalid"));
	_npcManager->PopulateNPCData(GetWorld());

	_timeManager = NewObject<UTimeManager>();
	checkf(_timeManager, TEXT("Time Manager is invalid"));

	//Gets a reference to the Journal in the world
	_journal = Cast<AJournal>(UGameplayStatics::GetActorOfClass(GetWorld(), AJournal::StaticClass()));
	checkf(_journal, TEXT("Missing Journal Reference"));
	_journal->Init(_npcManager);
}
