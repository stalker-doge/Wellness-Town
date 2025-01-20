// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GOAP_Planner.generated.h"

class UGOAP_Action;
class UGOAP_Goal;
class UGOAP_Plan;
class UGOAP_Agent;
class UGOAP_Node;

UCLASS()
class UE_WELLNESS_TOWN_API UGOAP_Planner : public UObject
{
	GENERATED_BODY()
	
public:
	UGOAP_Planner();

	UGOAP_Plan* Plan(UGOAP_Agent* agent, TArray<UGOAP_Goal*> goals, UGOAP_Goal* lastGoal);

private:
	bool FindPath(UGOAP_Node* parent, TArray<UGOAP_Action*> actions);
};
