// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FishRankUI.h"
#include "Components/TextBlock.h"

void UFishRankUI::UpdateAndShow(const FCaughtFishInfo& FishInfo, float SustainTime)
{
	if (Text_FishName)
	{
		Text_FishName->SetText(FText::FromString(FishInfo.FishName));
	}
	if (Text_FishGrade)
	{
		Text_FishGrade->SetText(FText::FromString(FString::Printf(TEXT("%d"), FishInfo.Grade)));
	}
	if (Text_FishWeight)
	{
		Text_FishWeight->SetText(FText::FromString(FString::Printf(TEXT("%.2f kg"), FishInfo.Weight)));
	}

	ShowUIAnim(SustainTime);
}
