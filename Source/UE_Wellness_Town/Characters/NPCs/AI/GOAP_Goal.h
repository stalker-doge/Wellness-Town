// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GOAP_Goal.generated.h"

class UGOAP_Belief;

UCLASS()
class UE_WELLNESS_TOWN_API UGOAP_Goal : public UObject
{
	GENERATED_BODY()
	
public:
	UGOAP_Goal();
	bool CheckIfFulfilled();

	class UE_WELLNESS_TOWN_API Builder
	{
		TWeakObjectPtr<UGOAP_Goal> goal;

	public:
		Builder(FString name)
		{
			goal = NewObject<UGOAP_Goal>();
			goal->_name = name;
		}
		Builder WithPriority(int priority)
		{
			goal->_priority = priority;
			return *this;
		}
		Builder AddDesiredEffect(UGOAP_Belief* effect)
		{
			goal->_desiredEffects.Add(effect);
			return *this;
		}
		UGOAP_Goal* Build()
		{
			return goal.Get();
		}
	};

public:
	FString _name;
	int _priority;

	TArray<TObjectPtr<UGOAP_Belief>> _desiredEffects;
};
