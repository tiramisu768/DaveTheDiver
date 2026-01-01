// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainUI.h"
#include "UI/RoboHPBarUI.h"
#include "UI/RoboAimUI.h"
#include "UI/RoboWeaponUI.h"
#include "UI/WarningOxygenUI.h"
#include "UI/FishRankUI.h"
#include "UI/ResultTableUI.h"
#include "UI/ShopUI.h"
#include "MyRobo/MyRobo.h"
#include "Weapon/Weapon.h"
#include "Components/Image.h"
#include "Tool/ToolData.h"
#include "ActorComponent/InventoryComponent.h"
#include "ActorComponent/StateComponent/RoboComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "GameInstance/MyGameInstance.h"

void UMainUI::NativeConstruct()
{
	Super::NativeConstruct();

	if (AimWidget)
	{
		AimWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	if(RankWidget)
	{
		RankWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	if (WarningHPWidget)
	{
		WarningHPWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	if (ResultTableWidget)
	{
		ResultTableWidget->SetVisibility(ESlateVisibility::Hidden);
		ResultTableWidget->OnConfirmRequested.AddDynamic(this, &UMainUI::HandleResultConfirmRequested); //Confirm 요청을 메인이 받는다
		ResultTableWidget->OnResultConfirmed.AddDynamic(this, &UMainUI::HandleResultConfirmed); //Hide 애니 종료 후 흐름 수신
	}

	if (ShopWidget)
	{
		ShopWidget->SetVisibility(ESlateVisibility::Hidden);
		ShopWidget->OnConfirmRequested.AddDynamic(this, &UMainUI::HandleShopConfirmRequested);
		ShopWidget->OnShopClosed.AddDynamic(this, &UMainUI::HandleShopClosed);
	}

	if (MapCandidates.Num() == 0)
	{
		MapCandidates.Add(FName("Map1Level"));
		MapCandidates.Add(FName("Map2Level"));
	}

}

void UMainUI::SetHPPercent(float value)
{
	if(HPBarWidget)
	{
		HPBarWidget->SetHPBarPercent(value);
	}
}

void UMainUI::SetMeters(float value)
{
	if (HPBarWidget)
	{
		HPBarWidget->SetCurrentDepthMeters(value);
	}
}

void UMainUI::SetWeights(float Current, float Max)
{
	if (HPBarWidget)
	{
		HPBarWidget->SetCurrentAndMaxWeight(Current, Max);
	}
}

void UMainUI::OverWeightNotification(bool becameOverweight)
{
	if (HPBarWidget)
	{
		HPBarWidget->ShowOverWeight(becameOverweight);
	}
}

void UMainUI::ShowCollectedFishNotification(const FCaughtFishInfo& FishInfo)
{
	if (RankWidget)
	{
		RankWidget->SetVisibility(ESlateVisibility::Visible);
		RankWidget->ShowFishCollected(FishInfo);
	}
}

void UMainUI::ShowHPWarningWidget()
{
	if (WarningHPWidget)
	{
		WarningHPWidget->SetVisibility(ESlateVisibility::Visible);
		WarningHPWidget->SetOxygenWarning();
	}
}

void UMainUI::PlayWeaponSwitchAnimation(int32 SelectedIndex)
{
	if (EquipmentWidget)
	{
		EquipmentWidget->PlaySwitchRangedIconAnimation(SelectedIndex);
	}
}

void UMainUI::PlayToolSwitchAnimation(int32 SelectedIndex)
{
	if (EquipmentWidget)
	{
		EquipmentWidget->PlaySwitchToolIconAnimation(SelectedIndex);
	}
}

void UMainUI::OnUpdateWeaponSlot(EWeaponSlot WeaponSlot, AWeapon* NewWeapon)
{
	if (!NewWeapon || !EquipmentWidget) return;

	if (const FWeaponData* WeaponData = NewWeapon->GetWeaponStats())
	{
		if (UTexture2D* IconTexture = WeaponData->Icon.LoadSynchronous())
		{
			EquipmentWidget->UpdateWeaponIcon(WeaponSlot, IconTexture);
		}
	}
}

void UMainUI::OnUpdateToolSlot(int32 SlotIndex, FName NewToolRowName)
{
	if (!EquipmentWidget || !ToolDataTable) return;

	UTexture2D* Icon = nullptr;

	if (!NewToolRowName.IsNone())
	{
		if (const FToolData* ToolData = ToolDataTable->FindRow<FToolData>(NewToolRowName, TEXT("")))
		{
			Icon = ToolData->ToolIcon.LoadSynchronous();
		}
	}
	EquipmentWidget->UpdateToolIcon(SlotIndex, Icon);
}

void UMainUI::OnChangeActiveTool(int32 NewActiveSlotIndex)
{
	if (EquipmentWidget)
	{
		EquipmentWidget->PlaySwitchToolIconAnimation(NewActiveSlotIndex);
	}
}

void UMainUI::StartAiming()
{
	if (AimWidget)
	{
		AimWidget->SetVisibility(ESlateVisibility::Visible);
		AimWidget->ResetAimPos();
	}
}

void UMainUI::StopAiming()
{
	if (AimWidget)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("[MainUI] AimWidget is valid. Hiding widget."));
		}
		AimWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UMainUI::UpdateAimPos(FVector2D MoveDelta)
{
	if (AimWidget)
	{
		AimWidget->UpdateAimPos(MoveDelta);
	}
}

void UMainUI::ResetAimPos()
{
	if (AimWidget)
	{
		AimWidget->ResetAimPos();
	}
}

void UMainUI::StartRandomMap()
{
	if (MapCandidates.Num() == 0)
	{
		return;
	}

	const int32 Index = FMath::RandRange(0, MapCandidates.Num() - 1);
	const FName MapToLoad = MapCandidates[Index];
	UE_LOG(LogTemp, Log, TEXT("random map : %s"), *MapToLoad.ToString());

	if (UWorld* World = GetWorld())
	{
		UGameplayStatics::OpenLevel(World, MapToLoad);
	}
}

FVector2D UMainUI::GetCrosshairScreenPosition() const
{
	if (AimWidget)
	{
		return AimWidget->GetCrosshairScreenPosition();
	}
	return FVector2D::ZeroVector;
}

void UMainUI::HandleResultConfirmRequested(bool bSuccess, int32 SelectedIndex)
{
	UE_LOG(LogTemp, Log, TEXT("success=%d selectedIdx=%d"),bSuccess?1:0,SelectedIndex);

	if (APlayerController* PlayerController = GetOwningPlayer())
	{
		APawn* Pawn = PlayerController->GetPawn();
		if (Pawn)
		{
			if (UInventoryComponent* Inventory = Pawn->FindComponentByClass<UInventoryComponent>())
			{
				if (UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance()))
				{
					if (bSuccess)
					{
						int32 TotalPrice = 0;
						for (const FCaughtFishInfo& Fish : Inventory->GetCaughtFishList())
						{
							TotalPrice += Fish.Grade * 10 + static_cast<int32>(Fish.Weight * 5);
						}
						GI->AddCoins(TotalPrice);
						GI->SetTempCaughtFishList(Inventory->GetCaughtFishList());
						GI->RequestShowShopOnLobby();
					}
					else
					{
						const TArray<FCaughtFishInfo>& FishList = Inventory->GetCaughtFishList();
						if (FishList.IsValidIndex(SelectedIndex))
						{
							const FCaughtFishInfo& SelectedFish = FishList[SelectedIndex];
							const int32 Reward = SelectedFish.Grade * 10 + static_cast<int32>(SelectedFish.Weight * 5);
							GI->AddCoins(Reward);
							GI->SetSelectedFish(SelectedFish);
						}
					}
					Inventory->SellAllFish();
				}
			}
		}
		PlayerController->SetPause(false);
		PlayerController->SetShowMouseCursor(false);
	}

	if (ResultTableWidget)
	{
		ResultTableWidget->HideUI();
	}
}

void UMainUI::HandleResultConfirmed(bool bSuccess)
{
	UE_LOG(LogTemp, Log, TEXT("success=%d"),bSuccess?1:0);

	if (bSuccess && ShopWidget)
	{
		ShopWidget->ShowUI();
	}
	else
	{
		StartRandomMap();
	}
}

void UMainUI::HandleShopConfirmRequested()
{
	if (ShopWidget)
	{
		ShopWidget->HideUI();
	}
}

void UMainUI::HandleShopClosed()
{
	StartRandomMap();
}

void UMainUI::ShowGameResultUI(bool bSuccess)
{
	 if (WarningHPWidget)
	 {
		 WarningHPWidget->SetVisibility(ESlateVisibility::Hidden);
	 }

	if (ResultTableWidget)
	{
		ResultTableWidget->SetGameEnd(bSuccess);
		ResultTableWidget->ShowUI();
	}
}

void UMainUI::ShowShopUI()
{
	if (ShopWidget)
	{
		ShopWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

