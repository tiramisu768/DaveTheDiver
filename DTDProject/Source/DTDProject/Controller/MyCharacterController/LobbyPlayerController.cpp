// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/MyCharacterController/LobbyPlayerController.h"
#include "Blueprint/UserWidget.h"

ALobbyPlayerController::ALobbyPlayerController()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> LobbyWidgetFinder(TEXT("/Game/BluePrint/UI/BP_LobbyUI.BP_LobbyUI_C"));
	if (LobbyWidgetFinder.Succeeded())
	{
		LobbyWidgetClass = LobbyWidgetFinder.Class;
	}
}

void ALobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (LobbyWidgetClass)
	{
		LobbyWidgetInstance = CreateWidget<UUserWidget>(this, LobbyWidgetClass);
		if (LobbyWidgetInstance)
		{
			LobbyWidgetInstance->AddToViewport();
		}
	}

	bShowMouseCursor = true;

	FInputModeUIOnly InputMode;
	SetInputMode(InputMode);
}
