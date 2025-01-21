// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GOAP_Node.generated.h"

class UGOAP_Action;
class UGOAP_Belief;

UCLASS()
class UE_WELLNESS_TOWN_API UGOAP_Node : public UObject
{
	GENERATED_BODY()
	
public:
	UGOAP_Node();

	void Init(UGOAP_Node* nodeParent, UGOAP_Action* nodeAction, TArray<UGOAP_Belief*> nodeEffects, int nodeCost);
	bool IsDead();
public:

	TWeakObjectPtr<UGOAP_Node> parent;
	TWeakObjectPtr<UGOAP_Action> action;
	TArray<TObjectPtr<UGOAP_Belief>> requiredEffects;
	TArray<TObjectPtr<UGOAP_Node>> neighbours;
	int cost;
};
