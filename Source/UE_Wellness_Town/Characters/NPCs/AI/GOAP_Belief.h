// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GOAP_Belief.generated.h"

/**
 * 
 */

DECLARE_DELEGATE_RetVal(bool, FEvaluate);
UCLASS()
class UE_WELLNESS_TOWN_API UGOAP_Belief : public UObject
{
	GENERATED_BODY()
	
public:
	UGOAP_Belief();

	bool Evaluate();
	FString GetName();

	class UE_WELLNESS_TOWN_API Builder
	{
		TWeakObjectPtr<UGOAP_Belief> belief;

	public:
		Builder(FString name)
		{
			belief = NewObject<UGOAP_Belief>();
			belief->_name = name;
		}

		Builder WithCondition(TFunction<bool()> condition)
		{
			belief->_condition.BindLambda(condition);
			return *this;
		}
		Builder WithLocation(FVector location)
		{
			belief->_location = location;
			return *this;
		}

		UGOAP_Belief* Build()
		{
			return belief.Get();
		}
	};

public:
	FString _name;
	FEvaluate _condition;
	FVector _location;
};
