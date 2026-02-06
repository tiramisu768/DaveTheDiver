// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/InstantRewardBox.h"
#include "MyRobo/MyRobo.h"
#include "ActorComponent/StateComponent/RoboComponent.h"
#include "Weapon/Weapon.h"

void AInstantRewardBox::OnBoxOpened()
{
	if (!IsValid(CurrentInteractingRobo)) return;

	switch (RewardType)
	{
	case EInstantRewardType::Ammo:
		if (AWeapon* ActiveWeapon = CurrentInteractingRobo->GetGunWeapon())
		{
			ActiveWeapon->ReloadToMax();
			UE_LOG(LogTemp, Log, TEXT("[InstantRewardBox] Added ammo to %s"),*ActiveWeapon->GetName());
		}
		break;

	case EInstantRewardType::Oxygen:
		if (URoboComponent* RoboState = CurrentInteractingRobo->FindComponentByClass<URoboComponent>())
		{
			RoboState->RestoreOxygen(OxygenRefillAmount); //RestoreOxygen
			UE_LOG(LogTemp, Log, TEXT("[InstantRewardBox] Restored %f oxygen"),OxygenRefillAmount);
		}
		break;
	}
}
