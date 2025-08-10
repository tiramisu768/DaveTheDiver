// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/MyHUD.h"
#include "Blueprint/UserWidget.h"
#include "UI/RoboHPBarUI.h"

AMyHUD::AMyHUD()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("/Game/Blueprints/UI/BP_RoboHPBar.BP_RoboHPBar_C"));
	if (WidgetClass.Succeeded())
	{
		HPBarWidget = WidgetClass.Class;
	}
}

void AMyHUD::BeginPlay()
{
	GEngine->AddOnScreenDebugMessage(-3, 0.0f, FColor::Red, FString::Printf(TEXT("GetHUD: %d")));
	Super::BeginPlay();
	if (HPBarWidget)
	{
		RoboHPBar = CreateWidget<URoboHPBarUI>(GetWorld(), HPBarWidget);
		if (RoboHPBar)
		{
			GEngine->AddOnScreenDebugMessage(-3, 0.0f, FColor::Red, FString::Printf(TEXT("GetHUD: %d")));
			RoboHPBar->AddToViewport();
		}
	}
}

void AMyHUD::SetHPPercent(float value)
{
	RoboHPBar->SetHPBarPercent(value);
}