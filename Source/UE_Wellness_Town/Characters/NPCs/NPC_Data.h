// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NPC_Data.generated.h"

class UPaperSprite;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_WELLNESS_TOWN_API UNPC_Data : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UNPC_Data();

	int GetTrustScore();
	void AdjustTrustScore(int amount);

	int GetFriendshipScore();
	void AdjustFriendshipScore(int amount);

	void LearnName();
	void LearnLike(FString toLearn);
	void LearnDislike(FString toLearn);

	FString GetNPCName();
	TArray<FString> GetLikes();
	TArray<FString> GetDislikes();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Trust")
	int _minTrust = 0;
	UPROPERTY(EditAnywhere, Category = "Trust")
	int _maxTrust = 10;
	UPROPERTY(EditAnywhere, Category = "Trust")
	int _trust;

	UPROPERTY(EditAnywhere, Category = "Friendship")
	int _minFriendship = 0;
	UPROPERTY(EditAnywhere, Category = "Friendship")
	int _maxFriendship = 10;
	UPROPERTY(EditAnywhere, Category = "Friendship")
	int _friendship;

	UPROPERTY(EditAnywhere, Category = "Info")
	TObjectPtr<UPaperSprite> _profileImage;

	// Bool = isKnown
	UPROPERTY(EditAnywhere, Category = "Info")
	FString _name;
	UPROPERTY(EditAnywhere, Category = "Info")
	bool _isNameKnown = false;

	UPROPERTY(EditAnywhere, Category = "Info")
	TMap<FString, bool> _likes;
	UPROPERTY(EditAnywhere, Category = "Info")
	TMap<FString, bool> _dislikes;
};
