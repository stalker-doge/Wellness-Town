// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KeywordInfoSystem.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_WELLNESS_TOWN_API UKeywordInfoSystem : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UKeywordInfoSystem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "KeywordInfo")//the keyword
		FString Keyword;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "KeywordInfo")//the information about the keyword
		FString Info;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "KeywordInfo")//the image of the keyword
		UTexture2D* Image;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "KeywordInfo")//list of keywords
		TArray<FString> Keywords;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "KeywordInfo")//list of information
		TArray<FString> Infos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "KeywordInfo")//list of images
		TArray<UTexture2D*> Images;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "KeywordInfo")//the index of the keyword
		int32 KeywordIndex;


	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "KeywordInfo")//sets the keyword index
		void SetKeywordIndex(int32 Index);

	UFUNCTION(BlueprintCallable, Category = "KeywordInfo")//gets the keyword index
		int32 GetKeywordIndex();

	UFUNCTION(BlueprintCallable, Category = "KeywordInfo")//sets the keyword
		void SetKeyword(FString NewKeyword);

	UFUNCTION(BlueprintCallable, Category = "KeywordInfo")//gets the keyword
		FString GetKeyword();

	UFUNCTION(BlueprintCallable, Category = "KeywordInfo")//sets the info
		void SetInfo(FString NewInfo);

	UFUNCTION(BlueprintCallable, Category = "KeywordInfo")//gets the info
		FString GetInfo();

	UFUNCTION(BlueprintCallable, Category = "KeywordInfo")//sets the image
		void SetImage(UTexture2D* NewImage);


	UFUNCTION(BlueprintCallable, Category = "KeywordInfo")//gets the image
		UTexture2D* GetImage();

	//incremets the keyword index
	UFUNCTION(BlueprintCallable, Category = "KeywordInfo")
	void IncrementKeywordIndex();
};
