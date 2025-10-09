// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Overlay.h"
#include "RoboWeaponUI.generated.h"

/**
 * 
 */
UCLASS()
class DTDPROJECT_API URoboWeaponUI : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UOverlay> Ranged_Current;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOverlay> Ranged_Back;
	TArray<TObjectPtr<UOverlay>> WeaponOverlayArray;
public:
	void PlaySwitchAnimation(int32 SelectedIndex);
	
};
