// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/MyHUD.h"
#include "Blueprint/UserWidget.h"
#include "UI/RoboHPBarUI.h"
#include "UI/RoboAimUI.h"
#include "UI/RoboWeaponUI.h"
#include "UI/ResultTableUI.h"
#include "UI/WarningOxygenUI.h"
#include "UI/FishRankUI.h"
#include "Engine/Canvas.h"

AMyHUD::AMyHUD()
{

	static ConstructorHelpers::FClassFinder<UUserWidget> HPBarWidgetClassFinder(TEXT("/Game/Blueprint/UI/BP_RoboHPBar.BP_RoboHPBar_C"));
	if (HPBarWidgetClassFinder.Succeeded())
	{
		HPBarWidget = HPBarWidgetClassFinder.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> WarningOxygenWidgetClassFinder(TEXT("/Game/BluePrint/UI/BP_WarningOxygen.BP_WarningOxygen_C"));
	if (WarningOxygenWidgetClassFinder.Succeeded())
	{
		WarningOxygenWidget = WarningOxygenWidgetClassFinder.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> WeaponWidgetClassFinder(TEXT("/Game/BluePrint/UI/BP_WeaponUI.BP_WeaponUI_C"));
	if (WeaponWidgetClassFinder.Succeeded())
	{
		WeaponWidget = WeaponWidgetClassFinder.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> RankWidgetClassFinder(TEXT("/Game/BluePrint/UI/BP_Rank.BP_Rank_C"));
	if (RankWidgetClassFinder.Succeeded())
	{
		RankWidget = RankWidgetClassFinder.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> AimWidgetClassFinder(TEXT("/Game/BluePrint/UI/BP_Aim.BP_Aim_C"));
	if (AimWidgetClassFinder.Succeeded())
	{
		AimWidget = AimWidgetClassFinder.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> ResultTableWidgetClassFinder(TEXT("/Game/BluePrint/UI/BP_ResultTable.BP_ResultTable_C"));
	if (ResultTableWidgetClassFinder.Succeeded())
	{
		ResultTableWidget = ResultTableWidgetClassFinder.Class;
	}
}

void AMyHUD::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("RoboWeaponUI »ý¼ºµÊ: %s"), *GetNameSafe(RoboWeaponUIClass));

	if (HPBarWidget)
	{
		RoboHPBarUIClass = CreateWidget<URoboHPBarUI>(GetWorld(), HPBarWidget);
		if (RoboHPBarUIClass)
		{
			RoboHPBarUIClass->AddToViewport();
		}
	}

	if (WarningOxygenWidget)
	{
		WarningOxygenUIClass = CreateWidget<UWarningOxygenUI>(GetWorld(), WarningOxygenWidget);
		if (WarningOxygenUIClass)
		{
			WarningOxygenUIClass->AddToViewport();
			WarningOxygenUIClass->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (WeaponWidget)
	{
		RoboWeaponUIClass = CreateWidget<URoboWeaponUI>(GetWorld(), WeaponWidget);
		if (RoboWeaponUIClass)
		{
			RoboWeaponUIClass->AddToViewport();
		}
	}

	if (RankWidget)
	{
		RankUIClass = CreateWidget<UFishRankUI>(GetWorld(), RankWidget);
		if (RankUIClass)
		{
			RankUIClass->AddToViewport();
		}
	}

	if (AimWidget)
	{
		RoboAimUIClass = CreateWidget<URoboAimUI>(GetWorld(), AimWidget);
		if (RoboAimUIClass)
		{
			RoboAimUIClass->AddToViewport();
			RoboAimUIClass->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (ResultTableWidget)
	{
		ResultTableUIClass = CreateWidget<UResultTableUI>(GetWorld(), ResultTableWidget);
		if (ResultTableUIClass)
		{
			ResultTableUIClass->AddToViewport();
			ResultTableUIClass->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}


void AMyHUD::SetHPPercent(float value)
{
	RoboHPBarUIClass->SetHPBarPercent(value);
}

void AMyHUD::SetMeters(float value)
{
	RoboHPBarUIClass->SetCurrentDepthMeters(value);
}

void AMyHUD::SetWeights(float Current, float Max)
{
	RoboHPBarUIClass->SetCurrentAndMaxWeight(Current, Max);
}

void AMyHUD::ShowRankUI()
{
	RankUIClass->ShowRank();
}

void AMyHUD::ShowOxygenWarningUI()
{
	WarningOxygenUIClass->SetVisibility(ESlateVisibility::Visible);
	WarningOxygenUIClass->SetOxygenWarning();
}

void AMyHUD::PlaySwitchAnimation(int32 SelectedIndex)
{
	if (RoboWeaponUIClass)
	{
		RoboWeaponUIClass->PlaySwitchAnimation(SelectedIndex);
	}
}

void AMyHUD::UpdateAimPos(FVector2D AimPos)
{
	if (RoboAimUIClass)
	{
		RoboAimUIClass->UpdateAimPos(AimPos);
	}
}

void AMyHUD::ResetAimPos()
{
	if (RoboAimUIClass)
	{
		RoboAimUIClass->ResetAimPos();
	}
}

void AMyHUD::ShowGameEndUI()
{
	ResultTableUIClass->SetVisibility(ESlateVisibility::Visible);
	ResultTableUIClass->SetGameEnd();
}
