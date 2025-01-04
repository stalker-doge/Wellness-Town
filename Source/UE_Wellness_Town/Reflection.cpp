// Fill out your copyright notice in the Description page of Project Settings.


#include "Reflection.h"

// Sets default values
AReflection::AReflection()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AReflection::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AReflection::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AReflection::StartReflection()
{
	//sets the index to 0, loads the first question, and increments the index
	ReflectionIndex = 0;
	ReflectionQuestions[ReflectionIndex];
	ReflectionIndex++;
}

void AReflection::StartReflectionQuestion(TArray<FString> Question)
{
	//finds the question, sets the index to the question, loads the question, and increments the index
	ReflectionIndex = ReflectionQuestions.Find(Question[0]);
	ReflectionQuestions[ReflectionIndex];
	ReflectionIndex++;

}

FString AReflection::GetCurrentQuestion()
{
	return FString();
}

void AReflection::NextQuestion()
{
}

void AReflection::AddQuestion(FString NewQuestion)
{
}

void AReflection::EndReflection()
{
}

void AReflection::SetCurrentQuestion(FString NewQuestion)
{

}

void AReflection::SaveResponse(FString Response)
{
	//saves the player's response in memory
}

