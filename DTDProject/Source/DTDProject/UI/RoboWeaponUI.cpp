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
	if (!Icon)return;

	UImage* TargetIcon = nullptr;
	FVector2D DesiredSize(0, 0);

	switch (WeaponSlot)
	{
	case EWeaponSlot::Melee:
		TargetIcon = MeleeWeaponIcon;
		DesiredSize = FVector2D(57.0f, 48.0f);
		break;
	case EWeaponSlot::Harpoon:
		TargetIcon = HarpoonWeaponIcon;
		DesiredSize = FVector2D(100.0f, 43.0f);
		break;
	case EWeaponSlot::Gun:
		TargetIcon = GunWeaponIcon;
		DesiredSize = FVector2D(100.0f, 43.0f);
		break;
	}

	if (TargetIcon)
	{
		TargetIcon->SetBrushFromTexture(Icon);

		if (!DesiredSize.IsZero())
		{
			FSlateBrush Brush = TargetIcon->GetBrush();
			Brush.ImageSize = DesiredSize;
			TargetIcon->SetBrush(Brush);
		}
		TargetIcon->SetVisibility(ESlateVisibility::Visible);
	}
}
