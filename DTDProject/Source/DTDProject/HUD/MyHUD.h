// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ActorComponent/InventoryComponent.h"
#include "MyHUD.generated.h"

/**
 * 
 */
UCLASS()
class DTDPROJECT_API AMyHUD : public AHUD
{
	GENERATED_BODY()
private:
	//HP
	TSubclassOf<UUserWidget> HPBarWidget;
	TObjectPtr<class URoboHPBarUI> RoboHPBarUIClass;

	//Warning Oxygen
	TSubclassOf<UUserWidget> WarningOxygenWidget;
	TObjectPtr<class UWarningOxygenUI> WarningOxygenUIClass;

	//무기
	TSubclassOf<UUserWidget> WeaponWidget;
	TObjectPtr<class URoboWeaponUI> RoboWeaponUIClass;

	//잡은 물고기 정보
	TSubclassOf<UUserWidget> RankWidget;
	TObjectPtr<class UFishRankUI> RankUIClass;

	//퀘스트 정보

	//Aim
	TSubclassOf<UUserWidget> AimWidget;
	TObjectPtr<class URoboAimUI> RoboAimUIClass;

	//Result Table
	TSubclassOf<UUserWidget> ResultTableWidget;
	TObjectPtr<class UResultTableUI> ResultTableUIClass;

public:
	AMyHUD();
	void BeginPlay() override;
	void SetHPPercent(float Percent);
	void SetMeters(float Meter);
	void SetWeights(float Current, float Max);
	void ShowRankUI(const TArray<FCaughtFishInfo>& FishList);
	void ShowOxygenWarningUI();
	URoboWeaponUI* GetRoboWeaponUI() const { return RoboWeaponUIClass; }
	void PlaySwitchAnimation(int32 SelectedIndex);
	URoboAimUI* GetRoboAimUI() const { return RoboAimUIClass; }
	void UpdateAimPos(FVector2D AimPos);
	void ResetAimPos();
	void ShowGameEndUI();
};
