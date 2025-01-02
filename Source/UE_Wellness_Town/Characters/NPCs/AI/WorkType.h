// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "WorkType.generated.h"

UENUM(BlueprintType)
enum class WorkType : uint8
{
	Fishing UMETA(DisplayName = "Fishing"),
	Trading UMETA(DisplayName = "Trading"),
	BugCatching UMETA(DisplayName = "BugCatching")
};
