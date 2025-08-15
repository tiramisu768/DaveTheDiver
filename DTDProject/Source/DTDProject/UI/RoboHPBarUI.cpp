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
		RoboHPText->SetText(FText::FromString(FString::Printf(TEXT("%.0f%"), Percent * 100.0f)));
	}
	if (RoboDepthText)
	{
		RoboDepthText->SetText(FText::FromString(FString::Printf(TEXT("%00.0fm%")))); //잠수깊이를 매개변수로 받아야할까?
	}
	if (RoboDepthTitleText)
	{
		RoboDepthTitleText->SetText(FText::FromString(FString::Printf(TEXT("%")))); 
	}

}
