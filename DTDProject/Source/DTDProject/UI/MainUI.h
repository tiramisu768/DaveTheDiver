// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ActorComponent/InventoryComponent.h"
#include "Weapon/WeaponData.h"
#include "MainUI.generated.h"

class UDataTable;
class URoboHPBarUI;
class URoboWeaponUI;
class UFishRankUI;
class URoboAimUI;
class UWarningOxygenUI;
class AWeapon;
class UResultTableUI;
class UShopUI;

UCLASS()
class DTDPROJECT_API UMainUI : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitializeUI(class AMyRobo* Robo);
	void SetHPPercent(float value);
	void SetMeters(float value);
	void SetWeights(float Current, float Max);
	void OverWeightNotification(bool becameOverweight);
	UFUNCTION()
	void ShowCollectedFishNotification(const FCaughtFishInfo& FishInfo);
	void ShowHPWarningWidget();

	URoboWeaponUI* GetRoboWeaponUI() const { return EquipmentWidget; }
	void PlayWeaponSwitchAnimation(int32 SelectedIndex);
	void PlayToolSwitchAnimation(int32 SelectedIndex);
	UFUNCTION()
	void OnUpdateWeaponSlot(EWeaponSlot WeaponSlot, AWeapon* NewWeapon);
	void OnUpdateToolSlot(int32 SlotIndex, FName NewToolRowName);
	void OnChangeActiveTool(int32 NewActiveSlotIndex);

	URoboAimUI* GetRoboAimUI() const { return AimWidget; }
	void StartAiming();
	void StopAiming();
	void UpdateAimPos(FVector2D MoveDelta);
	FVector2D GetCrosshairScreenPosition()const;

	void ShowGameResultUI(bool bSuccess);
	void ShowShopUI();

protected:
	virtual void NativeConstruct() override;

	//HP
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<URoboHPBarUI> HPBarWidget;

	//장비
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<URoboWeaponUI> EquipmentWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UDataTable> ToolDataTable;

	//물고기 정보
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UFishRankUI> RankWidget;

	//퀘스트 정보

	//Aim
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<URoboAimUI> AimWidget;

	//Warning HP
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWarningOxygenUI> WarningHPWidget;

	//Result Table
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UResultTableUI> ResultTableWidget;

	//Shop UI
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UShopUI> ShopWidget;

private:
	void ResetAimPos();
};
