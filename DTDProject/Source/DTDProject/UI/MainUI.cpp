// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainUI.h"
#include "UI/RoboHPBarUI.h"
#include "UI/RoboAimUI.h"
#include "UI/RoboWeaponUI.h"
#include "UI/ResultTableUI.h"
#include "UI/WarningOxygenUI.h"
#include "UI/FishRankUI.h"
#include "Weapon/Weapon.h"
#include "Components/Image.h"

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

void UMainUI::PlaySwitchAnimation(int32 SelectedIndex)
{
	if (WeaponWidget)
	{
		WeaponWidget->PlaySwitchRangedIconAnimation(SelectedIndex);
	}
}

void UMainUI::OnUpdateWeaponSlot(EWeaponSlot WeaponSlot, AWeapon* NewWeapon)
{
	if (!NewWeapon || !WeaponWidget) return;

	// --- 로그 추가: MainUI가 이벤트를 수신했음을 기록합니다. ---
	UE_LOG(LogTemp, Log, TEXT("[MainUI] OnUpdateWeaponSlot received for slot %s."), *UEnum::GetValueAsString(WeaponSlot));

	if (const FWeaponData* WeaponData = NewWeapon->GetWeaponStats())
	{
		if (UTexture2D* IconTexture = WeaponData->Icon.LoadSynchronous())
		{
			// --- 로그 추가: WeaponWidget에 아이콘 업데이트를 요청함을 기록합니다. ---
			UE_LOG(LogTemp, Log, TEXT("[MainUI] Forwarding icon update to WeaponWidget."));
			WeaponWidget->UpdateWeaponIcon(WeaponSlot, IconTexture);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("[MainUI] IconTexture is null for weapon %s."), *NewWeapon->GetName());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[MainUI] WeaponData is null for weapon %s."), *NewWeapon->GetName());
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

void UMainUI::ShowGameEndUI()
{
	if (ResultTableWidget)
	{
		ResultTableWidget->SetVisibility(ESlateVisibility::Visible);
		ResultTableWidget->SetGameEnd();
	}
}

