// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WT_GameMode.generated.h"

class UNPC_Manager;
class AJournal;

UCLASS(Blueprintable)
class UE_WELLNESS_TOWN_API AWT_GameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AWT_GameMode();

	TObjectPtr<UNPC_Manager> GetNPCManager();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<AJournal> _journal;

	TObjectPtr<UNPC_Manager> _npcManager;
};