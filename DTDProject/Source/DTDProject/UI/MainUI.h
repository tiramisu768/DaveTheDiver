// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ActorComponent/InventoryComponent.h"
#include "Weapon/WeaponData.h"
#include "MainUI.generated.h"

class URoboHPBarUI;
class URoboWeaponUI;
class UFishRankUI;
class URoboAimUI;
class UWarningOxygenUI;
class UResultTableUI;
class AWeapon;

UCLASS()
class DTDPROJECT_API UMainUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	//HP
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<URoboHPBarUI> HPBarWidget;

	//무기
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<URoboWeaponUI> WeaponWidget;

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


private:
	/*UPROPERTY()
	UUserWidget* CurrentWidget;*/

public:
	/*UFUNCTION(BlueprintCallable)
	void ShowLobbyUI();

	UFUNCTION(BlueprintCallable)
	void ShowMainUI();

	void OnEnterGame();*/

	//void BeginPlay() override;
	//UUserWidget* GetCurrentWidget() const { return CurrentWidget; };
	void SetHPPercent(float value);
	void SetMeters(float value);
	void SetWeights(float Current, float Max);
	void ShowRankUI(const TArray<FCaughtFishInfo>& FishList);
	void ShowHPWarningWidget();
	URoboWeaponUI* GetRoboWeaponUI() const { return WeaponWidget; }
	void PlaySwitchAnimation(int32 SelectedIndex);
	UFUNCTION()
	void OnUpdateWeaponSlot(EWeaponSlot WeaponSlot, AWeapon* NewWeapon);
	URoboAimUI* GetRoboAimUI() const { return AimWidget; }
	void UpdateAimPos(FVector2D AimPos);
	void ResetAimPos();
	void ShowGameEndUI();


};
