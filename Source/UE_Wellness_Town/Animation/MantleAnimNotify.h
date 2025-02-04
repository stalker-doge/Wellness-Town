// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "MantleAnimNotify.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnNotifiedSigature);

UCLASS()
class UE_WELLNESS_TOWN_API UMantleAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

public:
	FOnNotifiedSigature OnNotified;
};
