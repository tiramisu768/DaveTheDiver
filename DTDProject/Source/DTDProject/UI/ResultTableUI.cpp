// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ResultTableUI.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"
#include "ActorComponent/InventoryComponent.h"
#include "GameInstance/MyGameInstance.h"
#include "ShopUI.h"

void UResultTableUI::SetGameEnd(bool bWasSuccessful)
{
	if (ResultSwitcher)
	{
		if (bWasSuccessful)
		{
			ResultSwitcher->SetActiveWidgetIndex(GoodResultIndex);
		}
		else
		{
			ResultSwitcher->SetActiveWidgetIndex(BadResultIndex);
		}
	}

	//실패창이면 인벤토리에서 물고기목록을 가져와서 BP에게 전달해 목록을 채운다.
	if (!bWasSuccessful)
	{
		SelectedFishIndex = -1;
		UpdateConfirmButtonState();

		if (APlayerController* Controller = GetOwningPlayer())
		{
			if (APawn* Pawn = Controller->GetPawn())
			{
				if (UInventoryComponent* InventoryComponent = Pawn->FindComponentByClass<UInventoryComponent>())
				{
					const TArray<FCaughtFishInfo>& FishList = InventoryComponent->GetCaughtFishList();
					PopulateFishList(FishList);
				}
				else
				{
					TArray<FCaughtFishInfo> Empty;
					PopulateFishList(Empty);
				}
			}
		}
	}
	UpdateConfirmButtonState();
}

void UResultTableUI::SelectFishAtIndex(int32 Index)
{
	SelectedFishIndex = Index;
	UpdateConfirmButtonState();
	UE_LOG(LogTemp, Log, TEXT("SelectedFishIndex : %d"),SelectedFishIndex);
}

void UResultTableUI::NativeConstruct()
{
	Super::NativeConstruct();

	if (ConfirmButton)
	{
		ConfirmButton->OnClicked.RemoveDynamic(this, &UResultTableUI::OnConfirmClicked);
		ConfirmButton->OnClicked.AddDynamic(this, &UResultTableUI::OnConfirmClicked);
	}

	OnHidden.AddDynamic(this, &UResultTableUI::BroadcastResultConfirmed);
	SelectedFishIndex = -1;
	UpdateConfirmButtonState();
}

void UResultTableUI::OnConfirmClicked()
{
	int32 ActiveIndex = ResultSwitcher ? ResultSwitcher->GetActiveWidgetIndex() : -1;
	bPendingSuccess = (ActiveIndex == GoodResultIndex);

	if (OnConfirmRequested.IsBound())
	{
		OnConfirmRequested.Broadcast(bPendingSuccess, SelectedFishIndex);
	}
}

void UResultTableUI::UpdateConfirmButtonState()
{
	if (!ConfirmButton) return;

	const bool bOnBadPage = (ResultSwitcher && ResultSwitcher->GetActiveWidgetIndex() == BadResultIndex);
	
	if (bOnBadPage)
	{
		ConfirmButton->SetIsEnabled(SelectedFishIndex != -1);
	}
	else
	{
		ConfirmButton->SetIsEnabled(true);
	}
}

void UResultTableUI::BroadcastResultConfirmed()
{
	if (OnResultConfirmed.IsBound())
	{
		OnResultConfirmed.Broadcast(bPendingSuccess);
	}
}
