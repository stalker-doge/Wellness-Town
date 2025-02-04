// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DialogueSystem.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_WELLNESS_TOWN_API UDialogueSystem : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDialogueSystem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")//what the speaker says
	FString DialogueText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")//index of the current line of dialogue
		int32 DialogueIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")//what the player says
		FString PlayerDialogueText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")//who is speaking
	FString SpeakerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")//what the player could say
	TArray<FString> DialogueOptions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")//what the speaker says
	TArray<FString> DialogueSpeakerLines;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")//what the says for question 1
		TArray<FString> DialogueQuestion1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")//what the says for question 2
		TArray<FString> DialogueQuestion2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")//what the says for question 3
		TArray<FString> DialogueQuestion3;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Dialogue")//starts the dialogue
	void StartDialogue();

	UFUNCTION(BlueprintCallable, Category = "Dialogue")//starts the dialogue from a specific question
		void StartDialogueQuestion(TArray<FString> Question);


	UFUNCTION(BlueprintCallable, Category = "Dialogue")//loads the next line of dialogue
	void NextLine();

	UFUNCTION(BlueprintCallable, Category = "Dialogue")//loads the next line of dialogue from a question
		void NextLineQuestion(TArray<FString> Question);

	UFUNCTION(BlueprintCallable, Category = "Dialogue")//sets the player dialogue text to the option selected
	void PlayerDialogue(int32 Option);

	UFUNCTION(BlueprintCallable, Category = "Dialogue")//ends the dialogue
	void EndDialogue();

	UFUNCTION(BlueprintCallable, Category = "Dialogue")//checks if the response is the dialogue wanted
		bool CheckResponse(FString Response);

	UFUNCTION(BlueprintCallable, Category = "Dialogue")//adds a line of dialogue
		void AddDialogueLine(FString Line);

	UFUNCTION(BlueprintCallable, Category = "Dialogue")//adds a line of dialogue for question 1
		void AddDialogueQuestion1(FString Line);

	UFUNCTION(BlueprintCallable, Category = "Dialogue")//adds a line of dialogue for question 2
		void AddDialogueQuestion2(FString Line);

	UFUNCTION(BlueprintCallable, Category = "Dialogue")//adds a line of dialogue for question 3
		void AddDialogueQuestion3(FString Line);

	UFUNCTION(BlueprintCallable, Category = "Dialogue")//adds a line of dialogue for the player
		void AddPlayerLine(FString Line);

	UFUNCTION(BlueprintCallable, Category = "Dialogue")//Change next line of dialogue
		void ChangeNextLine(FString Line);

	UFUNCTION(BlueprintCallable, Category = "Dialogue")//Start dialogue from a custom line
		void StartDialogueFromLine(FString Line);
};
