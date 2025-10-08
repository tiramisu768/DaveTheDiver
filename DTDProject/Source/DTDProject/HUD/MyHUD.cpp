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

	//static ConstructorHelpers::FClassFinder<UUserWidget> MeleeWidgetClassFinder(TEXT("/Game/BluePrint/UI/BP_Melee.BP_Melee_C"));
	static ConstructorHelpers::FClassFinder<UUserWidget> MeleeWidgetClassFinder(TEXT(""));
	if (MeleeWidgetClassFinder.Succeeded())
	{
		MeleeWidget = MeleeWidgetClassFinder.Class;
	}

	//static ConstructorHelpers::FClassFinder<UUserWidget> ToolWidgetClassFinder(TEXT("/Game/BluePrint/UI/BP_Tool.BP_Tool_C"));
	static ConstructorHelpers::FClassFinder<UUserWidget> ToolWidgetClassFinder(TEXT(""));
	if (ToolWidgetClassFinder.Succeeded())
	{
		ToolWidget = ToolWidgetClassFinder.Class;
	}

	//static ConstructorHelpers::FClassFinder<UUserWidget> RangedWidgetClassFinder(TEXT("/Game/BluePrint/UI/BP_Ranged.BP_Ranged_C"));
	static ConstructorHelpers::FClassFinder<UUserWidget> RangedWidgetClassFinder(TEXT(""));
	if (RangedWidgetClassFinder.Succeeded())
	{
		RangedWidget = RangedWidgetClassFinder.Class;
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
	if (HPBarWidget)
	{
		RoboHPBarUIClass = CreateWidget<URoboHPBarUI>(GetWorld(), HPBarWidget);
		if (RoboHPBarUIClass)
		{
			RoboHPBarUIClass->AddToViewport();
		}
	}

	if (MeleeWidget)
	{
		UUserWidget* UWMelee = CreateWidget<UUserWidget>(GetWorld(), MeleeWidget);
		if (UWMelee)
		{
			UWMelee->AddToViewport();
		}
	}

	if (ToolWidget)
	{
		UUserWidget* UWTool = CreateWidget<UUserWidget>(GetWorld(), ToolWidget);
		if (UWTool)
		{
			UWTool->AddToViewport();
		}
	}

	if (RangedWidget)
	{
		UUserWidget* UWRanged = CreateWidget<UUserWidget>(GetWorld(), RangedWidget);
		if (UWRanged)
		{
			UWRanged->AddToViewport();
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

void AMyHUD::SetAimAngle(float Angle)
{
	if (RoboAimUIClass)
	{
		RoboAimUIClass->SetAimAngle(Angle);
	}
}
