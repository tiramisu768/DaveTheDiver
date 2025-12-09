// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RoboHPBarUI.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void URoboHPBarUI::SetHPBarPercent(float Percent)
{
	if (HPBar)
	{
		HPBar->SetPercent(Percent);
	}
	if (HPText)
	{
		HPText->SetText(FText::FromString(FString::Printf(TEXT("%.0f%%"), Percent * 100.0f)));
	}
}

void URoboHPBarUI::SetCurrentDepthMeters(float Meter)
{
	if (SeaDepthText)
	{
		SeaDepthText->SetText(FText::FromString(FString::Printf(TEXT("%.1fm"), Meter)));
	}
}

void URoboHPBarUI::SetCurrentAndMaxWeight(float Current, float Max)
{
	if (InventoryCurrentTotalWeightText)
	{
		InventoryCurrentTotalWeightText->SetText(FText::FromString(FString::Printf(TEXT("%.1fm"), Current)));
	}
	if (InventoryMaxWeightText)
	{
		InventoryMaxWeightText->SetText(FText::FromString(FString::Printf(TEXT("%.1fm"), Max)));
	}
}
