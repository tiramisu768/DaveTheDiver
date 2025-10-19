// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/MyHUD.h"
#include "Blueprint/UserWidget.h"
#include "UI/RoboHPBarUI.h"
#include "Engine/Canvas.h"

AMyHUD::AMyHUD()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> HPBarWidgetClassFinder(TEXT("/Game/Blueprint/UI/BP_RoboHPBar.BP_RoboHPBar_C"));
	if (HPBarWidgetClassFinder.Succeeded())
	{
		HPBarWidget = HPBarWidgetClassFinder.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> WeaponWidgetClassFinder(TEXT("/Game/BluePrint/UI/BP_WeaponUI.BP_WeaponUI_C"));
	if (WeaponWidgetClassFinder.Succeeded())
	{
		WeaponWidget = WeaponWidgetClassFinder.Class;
	}

	//static ConstructorHelpers::FClassFinder<UUserWidget> RankWidgetClassFinder(TEXT("/Game/BluePrint/UI/BP_Rank2.BP_Rank2_C"));
	static ConstructorHelpers::FClassFinder<UUserWidget> RankWidgetClassFinder(TEXT(""));
	if (RankWidgetClassFinder.Succeeded())
	{
		RankWidget = RankWidgetClassFinder.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> AimWidgetClassFinder(TEXT("/Game/BluePrint/UI/BP_Aim.BP_Aim_C"));
	if (AimWidgetClassFinder.Succeeded())
	{
		AimWidget = AimWidgetClassFinder.Class;
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
		UUserWidget* UWRank = CreateWidget<UUserWidget>(GetWorld(), RankWidget);
		if (UWRank)
		{
			UWRank->AddToViewport();
		}
	}

	if (AimWidget)
	{
		RoboAimUIClass = CreateWidget<URoboAimUI>(GetWorld(), AimWidget);
		if (RoboAimUIClass)
		{
			RoboAimUIClass->AddToViewport();
			//RoboAimUIClass->SetArcInfo(ArcCenter, ArcRadius);
			//RoboAimUIClass->SetVisibility(ESlateVisibility::Hidden);
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

void AMyHUD::PlaySwitchAnimation(int32 SelectedIndex)
{
	if (RoboWeaponUIClass)
	{
		RoboWeaponUIClass->PlaySwitchAnimation(SelectedIndex);
	}
}

void AMyHUD::SetAimPos(FVector2D AimPos)
{
	if (RoboAimUIClass)
	{
		RoboAimUIClass->SetAimPos(AimPos);
	}
}
