// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Overlay.h"
#include "Weapon/WeaponData.h"
#include "RoboWeaponUI.generated.h"

class UImage; 
class UTextBlock;

UCLASS()
class DTDPROJECT_API URoboWeaponUI : public UUserWidget
{
	GENERATED_BODY()
public:
	void PlaySwitchToolIconAnimation(int32 SelectedIndex);
	void PlaySwitchRangedIconAnimation(int32 SelectedIndex);
	void UpdateToolIcon(int32 SlotIndex, UTexture2D* Icon);
	void UpdateWeaponIcon(EWeaponSlot WeaponSlot, UTexture2D* Icon);
	void SetBulletCount(int32 Count, bool bIsInfinite);

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> MeleeWeaponIcon;	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> FirstToolIcon;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> SecondToolIcon;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> HarpoonWeaponIcon;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> GunWeaponIcon;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> BulletCountText;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* ToolAnim;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* ToolAnim_Reverse;

	UPROPERTY(meta=(BindWidgetAnim), Transient)
	UWidgetAnimation* RangedAnim;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* RangedAnim_Reverse;
};
