// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RoboHPBarUI.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void URoboHPBarUI::SetHPBarPercent(float Percent)
{
	if (RoboHPBar)
	{
		RoboHPBar->SetPercent(Percent);
	}
	if (RoboHPText)
	{
		RoboHPText->SetText(FText::FromString(FString::Printf(TEXT("%.0f%%"), Percent * 100.0f)));
	}
}

void URoboHPBarUI::SetCurrentDepthMeters(float Meter)
{
	//if (RoboDepthTitleText)
	//{
	//	RoboDepthTitleText->SetText(FText::FromString(FString::Printf(TEXT("Normal Depth")))); 
	//}
	if (RoboDepthText)
	{
		RoboDepthText->SetText(FText::FromString(FString::Printf(TEXT("%.1fm"), Meter)));
	}
}

void URoboHPBarUI::SetCurrentAndMaxWeight(float Current, float Max)
{
	if (RoboCurrentWeightText)
	{
		RoboCurrentWeightText->SetText(FText::FromString(FString::Printf(TEXT("%.1fm"), Current)));
	}
	if (RoboMaxWeightText)
	{
		RoboMaxWeightText->SetText(FText::FromString(FString::Printf(TEXT("%.1fm"), Max)));
	}
}
