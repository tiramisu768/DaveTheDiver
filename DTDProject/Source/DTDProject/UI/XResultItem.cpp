// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/XResultItem.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UXResultItem::SetItemInfo(const FCaughtFishInfo& Info, int32 Index)
{
	ItemIndex = Index;
	
	if (!FishIconImage || !FishName || !FishGrade || !FishWeight) return;

	FishName->SetText(FText::FromString(Info.FishName));
	FishGrade->SetText(FText::AsNumber(Info.Grade));
	FishWeight->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), Info.Weight)));

	if (UTexture2D* IconTexture = Info.FishIcon.LoadSynchronous())
	{
		FishIconImage->SetBrushFromTexture(IconTexture);
	}
}

void UXResultItem::NotifyItemSelected()
{
	OnItemSelected.Broadcast(ItemIndex);
}
