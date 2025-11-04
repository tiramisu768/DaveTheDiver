// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ResponsiveUI.h"

void UResponsiveUI::ShowUIAnim(float SustainTime)
{
	GetWorld()->GetTimerManager().ClearTimer(SustainTimerHandle);

	if (ShowAnim)
	{
		PlayAnimation(ShowAnim);

		if (SustainTime > 0)
		{
			GetWorld()->GetTimerManager().SetTimer(SustainTimerHandle, this, &UResponsiveUI::HideUIAnim, SustainTime);
		}
	}
	else
	{
		SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UResponsiveUI::HideUIAnim()
{
	if (HideAnim)
	{
		PlayAnimation(HideAnim);
	}
	else
	{
		SetVisibility(ESlateVisibility::Collapsed);
	}
}
