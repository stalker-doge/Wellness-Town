// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUD.generated.h"

class UImage;

UCLASS()
class UE_WELLNESS_TOWN_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UImage> _minimap;
};
