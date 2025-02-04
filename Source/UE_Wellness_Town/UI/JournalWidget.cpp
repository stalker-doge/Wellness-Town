// Fill out your copyright notice in the Description page of Project Settings.


#include "JournalWidget.h"
#include "UE_Wellness_Town/Characters/NPCs/NPC_Data.h"
#include "UMG.h"

void UJournalWidget::Init(TArray<UNPC_Data*> data)
{
	index = 2;

	_npcData = data;
	UpdatePage();
}

void UJournalWidget::Close()
{
	FInputModeGameOnly inputMode;
	GetWorld()->GetFirstPlayerController<APlayerController>()->SetInputMode(inputMode);
	GetWorld()->GetFirstPlayerController<APlayerController>()->SetShowMouseCursor(false);

	RemoveFromParent();
}

void UJournalWidget::PreviousPage()
{
	if (index <= 2)
	{
		return;
	}

	index -= 2;
	UpdatePage();
}

void UJournalWidget::NextPage()
{
	if (index >= _maxPage)
	{
		return;
	}

	index += 2;
	UpdatePage();
}

//Updates page data, needs to manage the images (when added) and clear the last empty page if there are an odd number of NPCs
void UJournalWidget::UpdatePage()
{
	_maxPage = _npcData.Num();

	if (_maxPage % 2 != 0)
	{
		_maxPage += 1;
	}

	_name1->SetText(FText::FromString(" "));

	_like1_1->SetText(FText::FromString(" "));
	_like2_1->SetText(FText::FromString(" "));
	_like3_1->SetText(FText::FromString(" "));
	_like4_1->SetText(FText::FromString(" "));

	_dislike1_1->SetText(FText::FromString(" "));
	_dislike2_1->SetText(FText::FromString(" "));
	_dislike3_1->SetText(FText::FromString(" "));
	_dislike4_1->SetText(FText::FromString(" "));

	_name2->SetText(FText::FromString(" "));

	_like1_2->SetText(FText::FromString(" "));
	_like2_2->SetText(FText::FromString(" "));
	_like3_2->SetText(FText::FromString(" "));
	_like4_2->SetText(FText::FromString(" "));

	_dislike1_2->SetText(FText::FromString(" "));
	_dislike2_2->SetText(FText::FromString(" "));
	_dislike3_2->SetText(FText::FromString(" "));
	_dislike4_2->SetText(FText::FromString(" "));

	if (index - 1 <= _npcData.Num() == true && index - 1 >= 0)
	{
		TObjectPtr<UNPC_Data> data = _npcData[index - 2];

		if (data != nullptr)
		{
			_name1->SetText(FText::FromString(data->GetNPCName()));

			TArray<FString> check = data->GetLikes();

			for (int i = 0; i < check.Num(); i++)
			{
				switch (i)
				{
				case 0:
					_like1_1->SetText(FText::FromString(check[i]));
					break;
				case 1:
					_like2_1->SetText(FText::FromString(check[i]));
					break;
				case 2:
					_like3_1->SetText(FText::FromString(check[i]));
					break;
				case 3:
					_like4_1->SetText(FText::FromString(check[i]));
					break;
				}
			}

			check = data->GetDislikes();

			for (int i = 0; i < check.Num(); i++)
			{
				switch (i)
				{
				case 0:
					_dislike1_1->SetText(FText::FromString(check[i]));
					break;
				case 1:
					_dislike2_1->SetText(FText::FromString(check[i]));
					break;
				case 2:
					_dislike3_1->SetText(FText::FromString(check[i]));
					break;
				case 3:
					_dislike4_1->SetText(FText::FromString(check[i]));
					break;
				}
			}
		}
	}

	if (index <= _npcData.Num() == true && index >= 0)
	{
		TObjectPtr<UNPC_Data> data = _npcData[index - 1];

		if (data != nullptr)
		{
			_name2->SetText(FText::FromString(data->GetNPCName()));

			TArray<FString> check = data->GetLikes();

			for (int i = 0; i < check.Num(); i++)
			{
				switch (i)
				{
				case 0:
					_like1_2->SetText(FText::FromString(check[i]));
					break;
				case 1:
					_like2_2->SetText(FText::FromString(check[i]));
					break;
				case 2:
					_like3_2->SetText(FText::FromString(check[i]));
					break;
				case 3:
					_like4_2->SetText(FText::FromString(check[i]));
					break;
				}
			}

			check = data->GetDislikes();

			for (int i = 0; i < check.Num(); i++)
			{
				switch (i)
				{
				case 0:
					_dislike1_2->SetText(FText::FromString(check[i]));
					break;
				case 1:
					_dislike2_2->SetText(FText::FromString(check[i]));
					break;
				case 2:
					_dislike3_2->SetText(FText::FromString(check[i]));
					break;
				case 3:
					_dislike4_2->SetText(FText::FromString(check[i]));
					break;
				}
			}
		}
	}
}
