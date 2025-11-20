// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RoboWeaponUI.h"
#include "MainUI.h"
#include "Components/Image.h"

void URoboWeaponUI::PlaySwitchToolIconAnimation(int32 SelectedIndex)
{
	if (SelectedIndex == 0)
	{
		PlayAnimation(ToolAnim);
	}
	else if (SelectedIndex == 1)
	{
		PlayAnimation(ToolAnim_Reverse);
	}
}

void URoboWeaponUI::PlaySwitchRangedIconAnimation(int32 SelectedIndex)
{
	if (SelectedIndex == 0)
	{
		PlayAnimation(RangedAnim);
	}
	else if (SelectedIndex == 1)
	{
		PlayAnimation(RangedAnim_Reverse);
	}
}

void URoboWeaponUI::UpdateWeaponIcon(EWeaponSlot WeaponSlot, UTexture2D* Icon)
{
	UImage* TargetIcon = nullptr;

	switch (WeaponSlot)
	{
	case EWeaponSlot::Melee:
		TargetIcon = MeleeWeaponIcon;
		break;
	case EWeaponSlot::Tool:
		FirstToolIcon = SecondToolIcon;
		TargetIcon = FirstToolIcon;
		break;
	case EWeaponSlot::Harpoon:
		TargetIcon = HarpoonWeaponIcon;
		break;
	case EWeaponSlot::Gun:
		TargetIcon = GunWeaponIcon;
		break;
	}

	if (TargetIcon && Icon)
	{
		TargetIcon->SetBrushFromTexture(Icon);
		//TargetIcon->SetVisibility(ESlateVisibility::Visible);
	}
}
