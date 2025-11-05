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
	UPROPERTY(meta=(BindWidgetAnim), Transient)
	UWidgetAnimation* RangedAnim;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* RangedAnim_Reverse;

public:
	void PlaySwitchAnimation(int32 SelectedIndex);
	
};
