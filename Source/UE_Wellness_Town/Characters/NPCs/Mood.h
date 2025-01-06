// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Mood.generated.h"

UENUM(BlueprintType)
enum class Mood : uint8
{
	None UMETA(DisplayName = "None"),
	Happy UMETA(DisplayName = "Happy"),
	Sad UMETA(DisplayName = "Sad"),
	Content UMETA(DisplayName = "Content")
};
