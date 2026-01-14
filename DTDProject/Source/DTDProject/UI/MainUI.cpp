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
#include "Save/PlayerSave.h"

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
		UE_LOG(LogTemp, Log, TEXT("MainUI::NativeConstruct - ResultTableWidget valid: %s"), * ResultTableWidget->GetName());
		ResultTableWidget->SetVisibility(ESlateVisibility::Hidden);
		ResultTableWidget->OnConfirmRequested.AddDynamic(this, &UMainUI::HandleResultConfirmRequested); //Confirm 요청을 메인이 받는다
		ResultTableWidget->OnResultConfirmed.AddDynamic(this, &UMainUI::HandleResultConfirmed); //Hide 애니 종료 후 흐름 수신
		
		UE_LOG(LogTemp, Log, TEXT("MainUI::NativeConstruct - After binding: OnResultConfirmed.IsBound=%d, OnHidden.IsBound=%d"),
			ResultTableWidget->OnResultConfirmed.IsBound() ? 1 : 0,
			ResultTableWidget->OnHidden.IsBound() ? 1 : 0);
	
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MainUI::NativeConstruct - ResultTableWidget is null"));
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

	UPlayerSave* Save = nullptr;
	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))
	{
		USaveGame* Loaded = UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0);
		Save = Cast<UPlayerSave>(Loaded);
	}
	if (!Save)
	{
		Save = Cast<UPlayerSave>(UGameplayStatics::CreateSaveGameObject(UPlayerSave::StaticClass()));
	}

	if (APlayerController* PlayerController = GetOwningPlayer())
	{
		APawn* Pawn = PlayerController->GetPawn();
		if (Pawn)
		{
			if (UInventoryComponent* Inventory = Pawn->FindComponentByClass<UInventoryComponent>())
			{		
				if (bSuccess)
				{
					int32 TotalPrice = 0;
					for (const FCaughtFishInfo& Fish : Inventory->GetCaughtFishList())
					{
						TotalPrice += Fish.Grade * 10 + static_cast<int32>(Fish.Weight * 5);
					}
					Save->Coins += TotalPrice;
					Save->TempCaughtFishList = Inventory->GetCaughtFishList();

					Save->bHasSelectedFish = false;
				}
				else
				{
					const TArray<FCaughtFishInfo>& FishList = Inventory->GetCaughtFishList();
					if (FishList.IsValidIndex(SelectedIndex))
					{
						const FCaughtFishInfo& SelectedFish = FishList[SelectedIndex];
						const int32 Reward = SelectedFish.Grade * 10 + static_cast<int32>(SelectedFish.Weight * 5);
						Save->Coins += Reward;
						Save->SelectedFish = SelectedFish;
						Save->bHasSelectedFish = true;
					}
				}

				// 기존 동작: 인벤토리 비움
				Inventory->SellAllFish();
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
	UE_LOG(LogTemp, Log, TEXT("MainUI::HandleShopConfirmRequested called"));

	if (!ShopWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("MainUI::HandleShopConfirmRequested - ShopWidget is null"));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("MainUI::HandleShopConfirmRequested - ShopWidget name: %s, OnConfirmRequested bound?: %d"),
		*ShopWidget->GetName(), ShopWidget->OnConfirmRequested.IsBound() ? 1 : 0);

	// 기존 동작: MainUI가 Shop 닫음
	ShopWidget->HideUI();
}

void UMainUI::HandleShopClosed()
{
	if (MapCandidates.Num() == 0)
	{
		StartRandomMap();
		return;
	}

	const int32 Index = FMath::RandRange(0, MapCandidates.Num() - 1);
	const FName MapToLoad = MapCandidates[Index];
	UE_LOG(LogTemp, Log, TEXT("HandleShopClosed - selected map: %s"), *MapToLoad.ToString());
	SaveAndOpenLevel(MapToLoad);
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

void UMainUI::SaveAndOpenLevel(const FName MapName)
{   //savegame 생성/갱신
	UPlayerSave* Save = Cast<UPlayerSave>(UGameplayStatics::CreateSaveGameObject(UPlayerSave::StaticClass()));
	if (!Save)
	{
		UE_LOG(LogTemp, Warning, TEXT("Fail SaveAndOpenLevel"));
		return;
	}
	//현재 인벤토리 데이터 저장
	if (APlayerController* Controller = GetOwningPlayer())
	{
		if (APawn* Pawn = Controller->GetPawn())
		{
			if (UInventoryComponent* Inventory = Pawn->FindComponentByClass<UInventoryComponent>())
			{
				Save->TempCaughtFishList = Inventory->GetCaughtFishList();
			}
		}
	}

	//Coin채우기
	//슬롯에 저장
	//다음 맵 이름 저장
	Save->NextMapName = MapName;

	//파일에 저장
	if (!UGameplayStatics::SaveGameToSlot(Save, SaveSlotName, 0))
	{
		UE_LOG(LogTemp, Warning, TEXT("SaveAndOpenLevel fail"));
	}

	//로딩레벨로 전환
	UGameplayStatics::OpenLevel(GetWorld(), FName("LoadingLevel"));
}

bool UMainUI::LoadSaveProgress()
{
	if (!UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0)) return false;

	USaveGame* Loaded = UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0);
	UPlayerSave* PlayerSave = Cast<UPlayerSave>(Loaded);
	if (!PlayerSave) return false;

	UE_LOG(LogTemp, Log, TEXT("LoadSaveProgress coin %d, hasSelected %d"),PlayerSave->Coins,PlayerSave->bHasSelectedFish?1:0);

	return true;
}

