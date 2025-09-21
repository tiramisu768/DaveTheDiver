// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
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

	//무기
	TSubclassOf<UUserWidget> MeleeWidget;
	TSubclassOf<UUserWidget> ToolWidget;
	TSubclassOf<UUserWidget> RangedWidget;

	//잡은 물고기 정보
	TSubclassOf<UUserWidget> RankWidget;

	//퀘스트 정보

public:
	AMyHUD();
	void BeginPlay() override;
	void SetHPPercent(float Percent);
	void SetMeters(float Meter);
	void SetWeights(float Current, float Max);
};
