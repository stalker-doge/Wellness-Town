// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "NPC_Manager.generated.h"

class UNPC_Data;

UCLASS(Blueprintable)
class UE_WELLNESS_TOWN_API UNPC_Manager : public UObject
{
	GENERATED_BODY()
	
public:
	UNPC_Manager();

	void PopulateNPCData(TObjectPtr<UWorld> world);
	TArray<TObjectPtr<UNPC_Data>> GetNPCData();

private:
	UPROPERTY()
	TArray<TObjectPtr<UNPC_Data>> _npcData;
};