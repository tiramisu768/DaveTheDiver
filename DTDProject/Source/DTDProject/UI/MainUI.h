// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainUI.generated.h"

class URoboHPBarUI;
class URoboWeaponUI;
class UFishRankUI;

UCLASS()
class DTDPROJECT_API UMainUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	//HP
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<URoboHPBarUI> HPBarWidget;

	//무기
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<URoboWeaponUI> WeaponWidget;

	//물고기 정보
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UFishRankUI> RankWidget;

	////Warning Oxygen
	//TSubclassOf<UUserWidget> WarningOxygenWidget;
	//TObjectPtr<class UWarningOxygenUI> WarningOxygenUIClass;

	////퀘스트 정보

	////Aim
	//TSubclassOf<UUserWidget> AimWidget;
	//TObjectPtr<class URoboAimUI> RoboAimUIClass;

	////Result Table
	//TSubclassOf<UUserWidget> ResultTableWidget;
	//TObjectPtr<class UResultTableUI> ResultTableUIClass;



private:
	/*UPROPERTY()
	UUserWidget* CurrentWidget;*/

public:
	/*UFUNCTION(BlueprintCallable)
	void ShowLobbyUI();

	UFUNCTION(BlueprintCallable)
	void ShowMainUI();

	void OnEnterGame();*/

	/*void BeginPlay() override;
	UUserWidget* GetCurrentWidget() const { return CurrentWidget; };
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
	void ShowGameEndUI();*/


};
