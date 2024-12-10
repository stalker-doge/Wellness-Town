// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_Manager.h"
#include "UE_Wellness_Town/Characters/NPCs/NPC_Base.h"
#include "Kismet/GameplayStatics.h"

UNPC_Manager::UNPC_Manager()
{
}

void UNPC_Manager::PopulateNPCData(UWorld* world)
{
	TArray<AActor*> npcs;

	UGameplayStatics::GetAllActorsOfClass(world, ANPC_Base::StaticClass(), npcs);

	for (AActor* actor : npcs)
	{
		_npcData.Add(Cast<ANPC_Base>(actor)->GetNPCData());
	}

	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, FString::Printf(TEXT("%03d"), _npcData.Num()));
}

TArray<UNPC_Data*> UNPC_Manager::GetNPCData()
{
	return _npcData;
}
