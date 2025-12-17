// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ResultTableUI.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"
#include "ActorComponent/InventoryComponent.h"
#include "GameInstance/MyGameInstance.h"

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
//잡은 물고기 목록을 채우는 등의 공통 로직을 추가할 수 있음
}

void UResultTableUI::NativeConstruct()
{
	Super::NativeConstruct();

	if (ConfirmButton)
	{
		ConfirmButton->OnClicked.RemoveDynamic(this, &UResultTableUI::OnConfirmClicked);
		ConfirmButton->OnClicked.AddDynamic(this, &UResultTableUI::OnConfirmClicked);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ConfirmButton is null - move the button to BP_ResultTable and set 'Is Variable'"));
	}
}

void UResultTableUI::OnConfirmClicked()
{
	int32 ActiveIndex = ResultSwitcher ? ResultSwitcher->GetActiveWidgetIndex() : -1;
	if (ActiveIndex == GoodResultIndex)
	{
		UE_LOG(LogTemp, Log, TEXT("OnConfirmClicked:Good"));
		if (APlayerController* PlayerController = GetOwningPlayer())
		{
			APawn* Pawn = PlayerController->GetPawn();
			if (Pawn)
			{
				if (UInventoryComponent* InventoryComponent = Pawn->FindComponentByClass<UInventoryComponent>())
				{
					int32 TotalPrice = 0;
					for (const FCaughtFishInfo& Fish : InventoryComponent->GetCaughtFishList())
					{
						TotalPrice += Fish.Grade * 10 + static_cast<int32>(Fish.Weight * 5);
					}

					if (UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetGameInstance()))
					{
						GameInstance ->AddCoins(TotalPrice);
						GameInstance->SetTempCaughtFishList(InventoryComponent->GetCaughtFishList());
					}
					InventoryComponent->SellAllFish();
				}
			}
			PlayerController->SetPause(false);
			PlayerController->SetShowMouseCursor(false);
		}
		UGameplayStatics::OpenLevel(GetWorld(), FName("LobbyLevel"));
	}
	else if (ActiveIndex == BadResultIndex)
	{
		UE_LOG(LogTemp, Log, TEXT("Bad"));
		UE_LOG(LogTemp, Log, TEXT("OnConfirmClicked:Good"));
		APlayerController* PlayerController = GetOwningPlayer();
		if (PlayerController)
		{
			PlayerController->SetPause(false);
			PlayerController->SetShowMouseCursor(false);
		}
		UGameplayStatics::OpenLevel(GetWorld(), FName("LobbyLevel"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("OnConfirmClicked: Unknown active index %d"), ActiveIndex);
	}
}