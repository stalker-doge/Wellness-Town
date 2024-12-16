// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UE_Wellness_Town/Interfaces/ActionStrategy.h"
#include "GOAP_Action.generated.h"

class UGOAP_Belief;

UCLASS()
class UE_WELLNESS_TOWN_API UGOAP_Action : public UObject
{
	GENERATED_BODY()
	
public:
	UGOAP_Action();

	void Start(); //Start Strategy
	void Update(float deltaTime); //Evaluate Strategy and Beliefs
	void Stop(); //Stop Strategy

	bool IsComplete();

	class UE_WELLNESS_TOWN_API Builder
	{
		TObjectPtr<UGOAP_Action> action;

	public:

		Builder(FString name)
		{
			action = NewObject<UGOAP_Action>();
			action->_name = name;
		}
		Builder WithCost(int cost)
		{
			action->_cost = cost;
			return *this;
		}
		Builder WithStrategy(IActionStrategy* strategy)
		{
			action->_strategy = strategy;
			return *this;
		}
		Builder AddPrecondition(UGOAP_Belief* precondition)
		{
			action->_preconditions.Add(precondition);
			return *this;
		}
		Builder AddEffect(UGOAP_Belief* effect)
		{
			action->_effects.Add(effect);
			return *this;
		}
		UGOAP_Action* Build()
		{
			return action;
		}
	};
public:
	IActionStrategy* _strategy;

	TArray<TObjectPtr<UGOAP_Belief>> _preconditions;
	TArray<TObjectPtr<UGOAP_Belief>> _effects;

	FString _name;
	int _cost;
};
