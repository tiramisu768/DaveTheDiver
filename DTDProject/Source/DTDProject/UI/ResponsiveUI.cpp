// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ResponsiveUI.h"

void UResponsiveUI::ShowUI()
{
	GetWorld()->GetTimerManager().ClearTimer(SustainTimerHandle);

	SetVisibility(ESlateVisibility::Visible);

	if (ShowAnim)
	{
		PlayAnimation(ShowAnim);
	}
	else
	{
		OnShow(); //blueprint event 호출
	}
}

void UResponsiveUI::ShowNotification(float SustainTime)
{
	ShowUI();

	if (SustainTime > 0)
	{
		GetWorld()->GetTimerManager().SetTimer(SustainTimerHandle, this, &UResponsiveUI::HideUI, SustainTime);
	}
}

void UResponsiveUI::HideUI()
{
	if (HideAnim)
	{
		FWidgetAnimationDynamicEvent EndDelegate;
		EndDelegate.BindUFunction(this, FName("OnHideAnimationFinished"));
		BindToAnimationFinished(HideAnim, EndDelegate);

		PlayAnimation(HideAnim, 0.0f, 1, EUMGSequencePlayMode::Forward, 1.0f, false);
	}
	else
	{
		SetVisibility(ESlateVisibility::Collapsed);
		OnHide();
		OnHidden.Broadcast();
	}
}

void UResponsiveUI::OnHideAnimationFinished()
{
	OnHide(); // blueprint event 호출
	OnHidden.Broadcast();
	SetVisibility(ESlateVisibility::Collapsed);
	
}
