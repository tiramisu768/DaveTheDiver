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
	}

	if (ShopWidget)
	{
		ShopWidget->SetVisibility(ESlateVisibility::Hidden);
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

FVector2D UMainUI::GetCrosshairScreenPosition() const
{
	if (AimWidget)
	{
		return AimWidget->GetCrosshairScreenPosition();
	}
	return FVector2D::ZeroVector;
}

void UMainUI::ShowGameResultUI(bool bSuccess)
{
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

