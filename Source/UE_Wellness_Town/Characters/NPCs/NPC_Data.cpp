// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_Data.h"

// Sets default values for this component's properties
UNPC_Data::UNPC_Data()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

int UNPC_Data::GetTrustScore()
{
	return _trust;
}

void UNPC_Data::AdjustTrustScore(int amount)
{
	_trust = FMath::Clamp(_trust + amount, _minTrust, _maxTrust);
}

int UNPC_Data::GetFriendshipScore()
{
	return _friendship;
}

void UNPC_Data::AdjustFriendshipScore(int amount)
{
	_friendship = FMath::Clamp(_friendship + amount, _minFriendship, _maxFriendship);
}

void UNPC_Data::LearnName()
{
	_isNameKnown = true;
}

void UNPC_Data::LearnLike(FString toLearn)
{
	if (_likes.Contains(toLearn) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s is an invalid like"), *toLearn);
		return;
	}

	_likes[toLearn] = true;
}

void UNPC_Data::LearnDislike(FString toLearn)
{
	if (_dislikes.Contains(toLearn) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s is an invalid dislike"), *toLearn);
		return;
	}

	_dislikes[toLearn] = true;
}

FString UNPC_Data::GetNPCName()
{
	if (_isNameKnown == false)
	{
		return "Unknown";
	}

	return _name;
}

TArray<FString> UNPC_Data::GetLikes()
{
	TArray<FString> validatedLikes;

	for (TPair<FString, bool>& pair : _likes)
	{
		if (pair.Value == false)
		{
			validatedLikes.Add("Unknown");
			continue;
		}

		validatedLikes.Add(pair.Key);
	}

	return validatedLikes;
}

TArray<FString> UNPC_Data::GetDislikes()
{
	TArray<FString> validatedDislikes;

	for (TPair<FString, bool>& pair : _dislikes)
	{
		if (pair.Value == false)
		{
			validatedDislikes.Add("Unknown");
			continue;
		}

		validatedDislikes.Add(pair.Key);
	}

	return validatedDislikes;
}

// Called when the game starts
void UNPC_Data::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

