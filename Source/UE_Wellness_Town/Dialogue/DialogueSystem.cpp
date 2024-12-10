// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueSystem.h"

// Sets default values for this component's properties
UDialogueSystem::UDialogueSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;



}


// Called when the game starts
void UDialogueSystem::BeginPlay()
{
	Super::BeginPlay();
	

}


// Called every frame
void UDialogueSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UDialogueSystem::StartDialogue()
{
	//sets the index to 0
	DialogueIndex = 0;
	//calls the event
	OnDialogueStart();
	//loads the first line of dialogue
	DialogueText = DialogueSpeakerLines[DialogueIndex];
	//increments the index
	DialogueIndex++;

}

void UDialogueSystem::NextLine()
{
	//loads the next line of dialogue
	DialogueText = DialogueSpeakerLines[DialogueIndex];
	//increments the index
	DialogueIndex++;
	//calls the event
	OnNextLine();

}

void UDialogueSystem::PlayerDialogue(int32 Option)
{
	//sets the player dialogue text to the option selected
	PlayerDialogueText = DialogueOptions[Option];
	//calls the event
	OnPlayerDialogue();
	//calls the event
	OnDialogueOptions();
	//calls the event
	OnNextLine();

}

void UDialogueSystem::EndDialogue()
{

}

void UDialogueSystem::OnDialogueStart_Implementation()
{
}

void UDialogueSystem::OnDialogueEnd_Implementation()
{
}

void UDialogueSystem::OnNextLine_Implementation()
{
}

void UDialogueSystem::OnPlayerDialogue_Implementation()
{
}

void UDialogueSystem::OnDialogueOptions_Implementation()
{
}
