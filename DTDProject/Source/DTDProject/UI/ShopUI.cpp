// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ShopUI.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "TimerManager.h"
#include "Engine/World.h"

void UShopUI::NativeConstruct()
{
	Super::NativeConstruct();

	if (ConfirmButton)
	{
		ConfirmButton->OnClicked.RemoveDynamic(this, &UShopUI::OnConfirmClicked);
		ConfirmButton->OnClicked.AddDynamic(this, &UShopUI::OnConfirmClicked);
		UE_LOG(LogTemp, Log, TEXT("%s::NativeConstruct - ConfirmButton bound"), *GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s::NativeConstruct - ConfirmButton is null! Make sure the button in BP has 'Is Variable' checked and name matches."), *GetName());
	}

	OnHidden.AddDynamic(this, &UShopUI::BroadcastShopClosed);
	UE_LOG(LogTemp, Log, TEXT("%s::NativeConstruct - OnHidden bound?: %d"), *GetName(), OnHidden.IsBound() ? 1 : 0);
}

void UShopUI::UpdateCoinCount(int32 NewCoinAmount)
{
	if (CoinText)
	{
		CoinText->SetText(FText::AsNumber(NewCoinAmount));
	}
}

void UShopUI::OnConfirmClicked()
{
	UE_LOG(LogTemp, Log, TEXT("%s::OnConfirmClicked called"), *GetName());

	if (OnConfirmRequested.IsBound())
	{
		UE_LOG(LogTemp, Log, TEXT("%s::OnConfirmClicked - broadcasting OnConfirmRequested"), *GetName());
		OnConfirmRequested.Broadcast();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s::OnConfirmClicked - OnConfirmRequested not bound!"), *GetName());
	}
}

void UShopUI::BroadcastShopClosed()
{
	OnShopClosed.Broadcast();
}