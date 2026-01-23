// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ActorComponent/InventoryComponent.h"
#include "Weapon/WeaponData.h"
#include "MainUI.generated.h"

class AMyRobo;
class UDataTable;
class URoboHPBarUI;
class URoboWeaponUI;
class UFishRankUI;
class URoboAimUI;
class UWarningOxygenUI;
class AWeapon;
class APickupItem;
class UResultTableUI;
class UShopUI;

UCLASS()
class DTDPROJECT_API UMainUI : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetHPPercent(float value);
	void SetMeters(float value);
	void SetWeights(float Current, float Max);
	void OverWeightNotification(bool becameOverweight);
	UFUNCTION()
	void ShowCollectedFishNotification(const FCaughtFishInfo& FishInfo);
	void ShowHPWarningWidget(bool bIsWarning);

	URoboWeaponUI* GetRoboWeaponUI() const { return EquipmentWidget; }
	void PlayWeaponSwitchAnimation(int32 SelectedIndex);
	void PlayToolSwitchAnimation(int32 SelectedIndex);
	UFUNCTION()
	void UpdateWeaponSlot(EWeaponSlot WeaponSlot, AWeapon* Weapon);
	UFUNCTION()
	void UpdateGunAmmo(AWeapon* Sender, int32 CurrentAmmo, float CooldownPercent);
	void OnUpdateToolSlot(int32 SlotIndex, FName NewToolRowName);
	void OnChangeActiveTool(int32 NewActiveSlotIndex);

	URoboAimUI* GetRoboAimUI() const { return AimWidget; }
	void StartAiming();
	void StopAiming();
	void UpdateAimPos(FVector2D MoveDelta);
	FVector2D GetCrosshairScreenPosition()const;

	UFUNCTION()
	void HandleResultConfirmRequested(bool bSuccess, int32 SelectedIndex);

	UFUNCTION()
	void HandleResultConfirmed(bool bSuccess);

	UFUNCTION()
	void HandleShopConfirmRequested();

	UFUNCTION()
	void HandleShopClosed();

	void ShowGameResultUI(bool bSuccess);
	void ShowShopUI();

	UFUNCTION(BlueprintCallable)
	void SaveAndOpenLevel(const FName MapName);

	UFUNCTION(BlueprintCallable)
	bool LoadSaveProgress();

protected:
	virtual void NativeConstruct() override;

	//HP
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<URoboHPBarUI> HPBarWidget;

	//장비
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<URoboWeaponUI> EquipmentWidget;

	UPROPERTY()
	TWeakObjectPtr<AWeapon> BoundRangedWeapon;

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

	//Loading
	UPROPERTY(EditDefaultsOnly,Category="Loading")
	TSubclassOf<UUserWidget> LoadingWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category ="Save")
	FString SaveSlotName = TEXT("PlayerSaveSlot");

	//맵 후보
	UPROPERTY(EditDefaultsOnly, Category="Flow")
	TArray<FName> MapCandidates;

private:
	void ResetAimPos();
	void StartRandomMap();
};
