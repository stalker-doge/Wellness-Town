// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UE_Wellness_Town/Interfaces/Interactable.h"
#include "Journal.generated.h"

class UJournalWidget;
class UNPC_Data;
class UNPC_Manager;
class UStaticMeshComponent;

UCLASS()
class UE_WELLNESS_TOWN_API AJournal : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJournal();
	void Init(TObjectPtr<UNPC_Manager> npcmanager);

	virtual void Interact(TObjectPtr<APlayerCharacter> player) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	int _maxPage;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> _staticMesh;

	TObjectPtr<UNPC_Manager> _npcManager;

 	UPROPERTY(EditAnywhere)
	TSubclassOf<UJournalWidget> _journalWidgetDefault;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UJournalWidget> _journalWidget;
};
