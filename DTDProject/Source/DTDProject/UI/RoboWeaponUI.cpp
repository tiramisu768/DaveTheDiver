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
	if (!Icon)
	{
		UE_LOG(LogTemp, Warning, TEXT("[RoboWeaponUI] Received null icon for slot %s."), *UEnum::GetValueAsString(WeaponSlot));
		return;
	}

	// --- 로그 추가: 아이콘 업데이트를 시작함을 기록합니다. ---
	UE_LOG(LogTemp, Log, TEXT("[RoboWeaponUI] Updating icon for slot: %s"), *UEnum::GetValueAsString(WeaponSlot));

	switch (WeaponSlot)
	{
	case EWeaponSlot::Melee:
		if (MeleeWeaponIcon)
		{
			MeleeWeaponIcon->SetBrushFromTexture(Icon);
			MeleeWeaponIcon->SetVisibility(ESlateVisibility::Visible);
			// --- 로그 추가: 성공 ---
			UE_LOG(LogTemp, Log, TEXT("[RoboWeaponUI] MeleeWeaponIcon updated successfully."));
		}
		break;
	case EWeaponSlot::Harpoon:
		if (HarpoonWeaponIcon)
		{
			HarpoonWeaponIcon->SetBrushFromTexture(Icon);
			HarpoonWeaponIcon->SetVisibility(ESlateVisibility::Visible);
			// --- 로그 추가: 성공 ---
			UE_LOG(LogTemp, Log, TEXT("[RoboWeaponUI] HarpoonWeaponIcon updated successfully."));
		}
		break;
	case EWeaponSlot::Gun:
		if (GunWeaponIcon)
		{
			GunWeaponIcon->SetBrushFromTexture(Icon);
			GunWeaponIcon->SetVisibility(ESlateVisibility::Visible);
			// --- 로그 추가: 성공 ---
			UE_LOG(LogTemp, Log, TEXT("[RoboWeaponUI] GunWeaponIcon updated successfully."));
		}
		break;
	}
}
