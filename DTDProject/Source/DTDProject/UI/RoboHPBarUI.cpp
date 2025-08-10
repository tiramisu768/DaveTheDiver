// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RoboHPBarUI.h"
#include "Components/ProgressBar.h"

void URoboHPBarUI::SetHPBarPercent(float Percent)
{
	if (RoboProgressBar)
	{
		RoboProgressBar->SetPercent(Percent);
	}
}
