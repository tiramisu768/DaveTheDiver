// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/MyHUD.h"
#include "Blueprint/UserWidget.h"
#include "UI/RoboHPBarUI.h"

AMyHUD::AMyHUD()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("/Game/Blueprint/UI/BP_RoboHPBar.BP_RoboHPBar_C"));
	if (WidgetClass.Succeeded())
	{
		HPBarWidget = WidgetClass.Class;
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
}

void AMyHUD::SetHPPercent(float value)
{
	RoboHPBarUIClass->SetHPBarPercent(value);
}