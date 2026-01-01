// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ShopUI.h"
#include "Components/Button.h"
#include "TimerManager.h"
#include "Engine/World.h"

void UShopUI::NativeConstruct()
{
	Super::NativeConstruct();

	if (ConfirmButton)
	{
		ConfirmButton->OnClicked.RemoveDynamic(this, &UShopUI::OnConfirmClicked);
		ConfirmButton->OnClicked.AddDynamic(this, &UShopUI::OnConfirmClicked);
	}
}

void UShopUI::OnConfirmClicked()
{
	if (OnConfirmRequested.IsBound())
	{
		OnConfirmRequested.Broadcast();
	}
}

void UShopUI::BroadcastShopClosed()
{
	OnShopClosed.Broadcast();
}