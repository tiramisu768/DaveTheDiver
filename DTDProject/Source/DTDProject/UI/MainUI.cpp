// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainUI.h"
#include "UI/RoboHPBarUI.h"
#include "UI/RoboAimUI.h"
#include "UI/RoboWeaponUI.h"
#include "UI/ResultTableUI.h"
#include "UI/WarningOxygenUI.h"
#include "UI/FishRankUI.h"

void UMainUI::NativeConstruct()
{
	Super::NativeConstruct();

	if (AimWidget)
	{
		AimWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	if (WarningHPWidget)
	{
		WarningHPWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	if (ResultTableWidget)
	{
		ResultTableWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UMainUI::SetHPPercent(float value)
{
	if(HPBarWidget)
	{
		HPBarWidget->SetHPBarPercent(value);
	}
}

void UMainUI::SetMeters(float value)
{
	if (HPBarWidget)
	{
		HPBarWidget->SetCurrentDepthMeters(value);
	}
}

void UMainUI::SetWeights(float Current, float Max)
{
	if (HPBarWidget)
	{
		HPBarWidget->SetCurrentAndMaxWeight(Current, Max);
	}
}

void UMainUI::ShowRankUI(const TArray<FCaughtFishInfo>& FishList)
{
	if(RankWidget)
	{
		/*RankWidget->UpdateFishRankList(FishList);
		RankWidget->UpdateAndShow();*/
	}
}

void UMainUI::ShowHPWarningWidget()
{
	if (WarningHPWidget)
	{
		WarningHPWidget->SetVisibility(ESlateVisibility::Visible);
		WarningHPWidget->SetOxygenWarning();
	}
}

void UMainUI::PlaySwitchAnimation(int32 SelectedIndex)
{
	if (WeaponWidget)
	{
		WeaponWidget->PlaySwitchAnimation(SelectedIndex);
	}
}

void UMainUI::UpdateAimPos(FVector2D AimPos)
{
	if (AimWidget)
	{
		AimWidget->UpdateAimPos(AimPos);
	}
}

void UMainUI::ResetAimPos()
{
	if (AimWidget)
	{
		AimWidget->ResetAimPos();
	}
}

void UMainUI::ShowGameEndUI()
{
	if (ResultTableWidget)
	{
		ResultTableWidget->SetVisibility(ESlateVisibility::Visible);
		ResultTableWidget->SetGameEnd();
	}
}

