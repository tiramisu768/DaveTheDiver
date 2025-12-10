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
		FishIcon->SetBrushFromSoftTexture(FishInfo.FishIcon, false);

		FSlateBrush Brush = FishIcon->GetBrush();
		Brush.ImageSize = FVector2D(60.0f, 60.0f);
		FishIcon->SetBrush(Brush);
	}

	ShowUIAnim(3.0f);
}
