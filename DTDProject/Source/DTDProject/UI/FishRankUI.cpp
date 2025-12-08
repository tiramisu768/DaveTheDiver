// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FishRankUI.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UFishRankUI::ShowFishCollected(const FCaughtFishInfo& FishInfo)
{
	if (FishNameText)
	{
		FishNameText->SetText(FText::FromString(FishInfo.FishName));
	}
	if (FishGradeText)
	{
		FishGradeText->SetText(FText::FromString(FString::Printf(TEXT("%d"), FishInfo.Grade)));
	}
	if (FishWeightText)
	{
		FishWeightText->SetText(FText::FromString(FString::Printf(TEXT("%.2f kg"), FishInfo.Weight)));
	}
	if (FishIcon)
	{

	}

	ShowUIAnim(3.0f);
}
