// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JournalWidget.generated.h"

class UTextBlock;
class UNPC_Data;
class UButton;

UCLASS()
class UE_WELLNESS_TOWN_API UJournalWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	void Init(TArray<UNPC_Data*> data);

	UFUNCTION(BlueprintCallable)
	void Close();
	UFUNCTION(BlueprintCallable)
	void PreviousPage();
	UFUNCTION(BlueprintCallable)
	void NextPage();

private:
	void UpdatePage();

private:
	int _maxPage;

	int index = 0;
	TArray<TObjectPtr<UNPC_Data>> _npcData;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UTextBlock> _name1;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UTextBlock> _like1_1;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UTextBlock> _like2_1;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UTextBlock> _like3_1;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UTextBlock> _like4_1;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UTextBlock> _dislike1_1;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UTextBlock> _dislike2_1;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UTextBlock> _dislike3_1;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UTextBlock> _dislike4_1;


	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UTextBlock> _name2;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UTextBlock> _like1_2;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UTextBlock> _like2_2;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UTextBlock> _like3_2;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UTextBlock> _like4_2;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UTextBlock> _dislike1_2;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UTextBlock> _dislike2_2;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UTextBlock> _dislike3_2;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UTextBlock> _dislike4_2;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UButton> _close;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UButton> _prevPage;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UButton> _nextPage;
};
