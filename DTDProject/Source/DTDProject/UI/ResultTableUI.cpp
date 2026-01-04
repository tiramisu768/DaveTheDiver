// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ResultTableUI.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"
#include "ActorComponent/InventoryComponent.h"
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

	if (APlayerController* Controller = GetOwningPlayer())
	{
		// 마우스 커서 보이기 및 UI 포커스 모드 설정
		Controller->bShowMouseCursor = true;

		// UI에 포커스를 맞추고 게임/UI 병행 모드로 설정 (안전하게 GameAndUI 사용)
		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		// TakeWidget() 를 사용해 이 위젯에 포커스 설정
		InputMode.SetWidgetToFocus(TakeWidget());
		Controller->SetInputMode(InputMode);

		// ConfirmButton이 존재하면 확실히 보이도록/클릭 가능하도록 설정
		if (ConfirmButton)
		{
			ConfirmButton->SetVisibility(ESlateVisibility::Visible);
			ConfirmButton->SetIsEnabled(true);
			UE_LOG(LogTemp, Log, TEXT("%s::SetGameEnd - ConfirmButton visibility/enabled forced for UI input"), *GetName());
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
		UE_LOG(LogTemp, Log, TEXT("%s::NativeConstruct - ConfirmButton bound: %s"), *GetName(), *ConfirmButton->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s::NativeConstruct - ConfirmButton is null"), *GetName());
	}

	OnHidden.AddDynamic(this, &UResultTableUI::BroadcastResultConfirmed);
	SelectedFishIndex = -1;
	UpdateConfirmButtonState();
}

void UResultTableUI::OnConfirmClicked()
{
	int32 ActiveIndex = ResultSwitcher ? ResultSwitcher->GetActiveWidgetIndex() : -1;
	UE_LOG(LogTemp, Log, TEXT("%s::OnConfirmClicked called - ActiveIndex=%d SelectedFishIndex=%d"),
		*GetName(), ActiveIndex, SelectedFishIndex);

	bPendingSuccess = (ActiveIndex == GoodResultIndex);

	if (OnConfirmRequested.IsBound())
	{
		OnConfirmRequested.Broadcast(bPendingSuccess, SelectedFishIndex);
		UE_LOG(LogTemp, Log, TEXT("%s::OnConfirmClicked - Broadcasted OnConfirmRequested (bPendingSuccess=%d)"),
			*GetName(), bPendingSuccess ? 1 : 0);
	}
}

void UResultTableUI::UpdateConfirmButtonState()
{
	if (!ConfirmButton)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s::UpdateConfirmButtonState - ConfirmButton is null"), *GetName());
		return;
	}

	const bool bOnBadPage = (ResultSwitcher && ResultSwitcher->GetActiveWidgetIndex() == BadResultIndex);
	int32 ActiveIndex = ResultSwitcher ? ResultSwitcher->GetActiveWidgetIndex() : -999;
	const ESlateVisibility Vis = ConfirmButton->GetVisibility();
	const bool bEnabled = ConfirmButton->GetIsEnabled();
	UE_LOG(LogTemp, Log, TEXT("%s::UpdateConfirmButtonState ActiveIndex=%d SelectedFishIndex=%d OnBad=%d ConfirmVis=%d ConfirmEnabled=%d"),
		*GetName(), ActiveIndex, SelectedFishIndex, bOnBadPage ? 1 : 0, (int)Vis, bEnabled ? 1 : 0);
	
	if (bOnBadPage)
	{
		ConfirmButton->SetIsEnabled(SelectedFishIndex != -1);
		UE_LOG(LogTemp, Log, TEXT("ConfirmButton SetIsEnabled -> %d"), SelectedFishIndex != -1);
	}
	else
	{
		ConfirmButton->SetIsEnabled(true);
		UE_LOG(LogTemp, Log, TEXT("ConfirmButton SetIsEnabled -> true (good page)"));
	}
}

void UResultTableUI::BroadcastResultConfirmed()
{
	if (OnResultConfirmed.IsBound())
	{
		OnResultConfirmed.Broadcast(bPendingSuccess);
	}
}
