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
	//loads the first line of dialogue
	DialogueText = DialogueSpeakerLines[DialogueIndex];
	//increments the index
	DialogueIndex++;

}

void UDialogueSystem::StartDialogueQuestion(TArray<FString> Question)
{
	//sets the index to 0
	DialogueIndex = 0;
	//loads the first line of dialogue
	DialogueText = Question[DialogueIndex];
	//increments the index
	DialogueIndex++;

}

void UDialogueSystem::NextLine()
{
	//makes sure the index is not out of bounds
	if (DialogueIndex >= DialogueSpeakerLines.Num())
	{
		//if it is out of bounds end the dialogue
		EndDialogue();
		return;
	}
	//loads the next line of dialogue
	DialogueText = DialogueSpeakerLines[DialogueIndex];
	//increments the index
	DialogueIndex++;

}

void UDialogueSystem::NextLineQuestion(TArray<FString> Question)
{
	//loads the next line of dialogue
	DialogueText = Question[DialogueIndex];
	//increments the index
	DialogueIndex++;
}

void UDialogueSystem::PlayerDialogue(int32 Option)
{
	//sets the player dialogue text to the option selected
	PlayerDialogueText = DialogueOptions[Option];

}

void UDialogueSystem::EndDialogue()
{
	//sets the index to 0
	DialogueIndex = 0;
	//clears the dialogue text
	DialogueText = "";

}

bool UDialogueSystem::CheckResponse(FString Response)
{
	//if the player response is the one wanted return true
	if (Response == PlayerDialogueText)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void UDialogueSystem::AddDialogueLine(FString Line)
{
	//adds a line of dialogue
	DialogueSpeakerLines.Add(Line);
}

void UDialogueSystem::AddDialogueQuestion1(FString Line)
{
	//adds a line of dialogue
	DialogueQuestion1.Add(Line);
}

void UDialogueSystem::AddDialogueQuestion2(FString Line)
{
	//adds a line of dialogue
	DialogueQuestion2.Add(Line);
}

void UDialogueSystem::AddDialogueQuestion3(FString Line)
{
	//adds a line of dialogue
	DialogueQuestion3.Add(Line);
}

void UDialogueSystem::AddPlayerLine(FString Line)
{
	//adds a line of dialogue
	DialogueOptions.Add(Line);
}

void UDialogueSystem::ChangeNextLine(FString Line)
{
	//changes the next line of dialogue
	DialogueText = Line;
	//increments the index
	DialogueIndex++;
}

void UDialogueSystem::StartDialogueFromLine(FString Line)
{
	//sets the index to 0
	DialogueIndex = 0;
	//loads the first line of dialogue
	DialogueText = Line;
	//increments the index
	DialogueIndex++;
}
