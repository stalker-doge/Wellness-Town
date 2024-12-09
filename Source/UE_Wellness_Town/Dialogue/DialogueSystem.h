// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DialogueSystem.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
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

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Dialogue")//starts the dialogue
	void StartDialogue();

	UFUNCTION(BlueprintCallable, Category = "Dialogue")//loads the next line of dialogue
	void NextLine();

	UFUNCTION(BlueprintCallable, Category = "Dialogue")//sets the player dialogue text to the option selected
	void PlayerDialogue(int32 Option);

	UFUNCTION(BlueprintCallable, Category = "Dialogue")//ends the dialogue
	void EndDialogue();

	UFUNCTION(BlueprintImplementableEvent, Category = "Dialogue")//custom event for starting dialogue
	void OnDialogueStart();

	UFUNCTION(BlueprintImplementableEvent, Category = "Dialogue")//custom event for ending dialogue
	void OnDialogueEnd();

	UFUNCTION(BlueprintImplementableEvent, Category = "Dialogue")//custom event for loading the next line of dialogue
	void OnNextLine();

	UFUNCTION(BlueprintImplementableEvent, Category = "Dialogue")//custom event for player dialogue
	void OnPlayerDialogue();

	UFUNCTION(BlueprintImplementableEvent, Category = "Dialogue")//custom event for dialogue options
	void OnDialogueOptions();

		
};
