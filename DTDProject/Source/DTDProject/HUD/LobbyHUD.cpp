// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/LobbyHUD.h"
#include "Blueprint/UserWidget.h"

ALobbyHUD::ALobbyHUD()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> LobbyWidgetBPClass(TEXT("/Game/BluePrint/UI/BP_LobbyUI.BP_LobbyUI_C"));
	if (LobbyWidgetBPClass.Succeeded())
		LobbyWidgetClass = LobbyWidgetBPClass.Class;
}

void ALobbyHUD::BeginPlay()
{
	Super::BeginPlay();

	if (LobbyWidgetClass)
	{
		LobbyWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), LobbyWidgetClass);
		if (LobbyWidgetInstance)
		{
			LobbyWidgetInstance->AddToViewport();
		}
	}
}
