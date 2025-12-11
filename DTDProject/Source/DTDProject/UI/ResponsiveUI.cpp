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
		// 1. 애니메이션이 끝났을 때 호출될 델리게이트를 생성합니다.
		FWidgetAnimationDynamicEvent EndDelegate;

		// 2. BindUFunction을 사용하여 UFUNCTION으로 선언된 멤버 함수에 바인딩합니다.
		EndDelegate.BindUFunction(this, FName("OnHideAnimationFinished"));

		// 3. PlayAnimation 함수의 마지막 인자로 델리게이트를 전달합니다.
		PlayAnimation(HideAnim, 0.0f, 1, EUMGSequencePlayMode::Forward, 1.0f, false);

		// 참고: PlayAnimation의 마지막 bool 파라미터(bRestoreState)가 true이면
		// 애니메이션 종료 후 상태가 원래대로 돌아가 UI가 다시 Visible이 될 수 있습니다.
		// 명시적으로 false로 설정하여 이를 방지합니다.
	}
	else
	{
		SetVisibility(ESlateVisibility::Collapsed);
		OnHide(); // blueprint event 호출
	}
}

void UResponsiveUI::OnHideAnimationFinished()
{
	SetVisibility(ESlateVisibility::Collapsed);
	OnHide(); // blueprint event 호출
}
