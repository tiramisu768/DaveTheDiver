// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/XResultUI.h"
#include "Components/VerticalBox.h"
#include "UI/XResultItem.h"

void UXResultUI::PopulateFishList(const TArray<struct FCaughtFishInfo>& FishList)
{
	if (!XResultVerticalBox || !XResultItem) return;

	XResultVerticalBox->ClearChildren();

	for (int32 i = 0; i < FishList.Num(); i++)
	{
		const FCaughtFishInfo& FishInfo = FishList[i];
		if (UXResultItem* Item = CreateWidget<UXResultItem>(this, XResultItem))
		{
			Item->SetItemInfo(FishInfo, i);
			Item->OnItemSelected.AddUniqueDynamic(this, &UXResultUI::HandleFishSelection);
			XResultVerticalBox->AddChild(Item);
		}
	}
}

void UXResultUI::HandleFishSelection(int32 Index)
{
	OnFishSelectedDelegate.ExecuteIfBound(Index);
}
