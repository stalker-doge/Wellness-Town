// Fill out your copyright notice in the Description page of Project Settings.


#include "KeywordInfoSystem.h"

// Sets default values for this component's properties
UKeywordInfoSystem::UKeywordInfoSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UKeywordInfoSystem::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UKeywordInfoSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UKeywordInfoSystem::SetKeywordIndex(int32 Index)
{
	KeywordIndex = Index;
}

int32 UKeywordInfoSystem::GetKeywordIndex()
{
	return KeywordIndex;
}

void UKeywordInfoSystem::SetKeyword(FString NewKeyword)
{
	Keyword = NewKeyword;

}

FString UKeywordInfoSystem::GetKeyword()
{
	return Keyword;
}

void UKeywordInfoSystem::SetInfo(FString NewInfo)
{
	Info = NewInfo;
}

FString UKeywordInfoSystem::GetInfo()
{
	return Info;
}

void UKeywordInfoSystem::SetImage(UTexture2D* NewImage)
{
	Image = NewImage;
}

UTexture2D* UKeywordInfoSystem::GetImage()
{
	return Image;
}

void UKeywordInfoSystem::IncrementKeywordIndex()
{
	KeywordIndex++;
}

