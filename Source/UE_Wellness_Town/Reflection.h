// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Reflection.generated.h"

UCLASS()
class UE_WELLNESS_TOWN_API AReflection : public AActor
{
	GENERATED_BODY()
	//system that enhances the current reflection system, where the player is presented with a question and has to choose between 5 predetermined responses
	
public:	
	// Sets default values for this actor's properties
	AReflection();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reflection")//array of questions
		TArray<FString> ReflectionQuestions;

	//enum for the different emotions the player can choose
	enum class Emotion
	{
		Angry,
		Sad,
		Happy,
		Confused,
		Neutral
	};
	TArray<Emotion> ReflectionEmotions;//array of emotions the player can choose

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reflection")//index of the current question
		int32 ReflectionIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reflection")//current question
		FString CurrentQuestion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reflection")//current response
		FString CurrentResponse;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Reflection")//starts the reflection
		void StartReflection();

	UFUNCTION(BlueprintCallable, Category = "Reflection")//starts the reflection from a specific question
		void StartReflectionQuestion(TArray<FString> Question);

	UFUNCTION(BlueprintCallable, Category = "Reflection")//gets the current question
		FString GetCurrentQuestion();

	UFUNCTION(BlueprintCallable, Category = "Reflection")//gets the next question
		void NextQuestion();

	UFUNCTION(BlueprintCallable, Category = "Reflection")//adds a new question
		void AddQuestion(FString NewQuestion);

	UFUNCTION(BlueprintCallable, Category = "Reflection")//ends the reflection
		void EndReflection();

	UFUNCTION(BlueprintCallable, Category = "Reflection")//sets the current question
		void SetCurrentQuestion(FString NewQuestion);

	UFUNCTION(BlueprintCallable, Category = "Reflection")//Saves the player's response, WIP 
		void SaveResponse(FString Response);

};
