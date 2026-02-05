// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/LootSpawnerBox.h"
#include "MyRobo/MyRobo.h"
#include "Weapon/Weapon.h"
#include "Weapon/WeaponData.h"
#include "Object/PickupItem.h"
#include "Engine/DataTable.h"
#include "Components/BoxComponent.h"

ALootSpawnerBox::ALootSpawnerBox()
{

}

void ALootSpawnerBox::SetSpawnedWeapon(AWeapon* NewWeapon)
{
	SpawnedWeapon = NewWeapon;
	if (SpawnedWeapon)
	{
		SpawnedWeapon->SetOwner(this);
	}
}

void ALootSpawnerBox::ClearSpawnedWeapon()
{
	SpawnedWeapon = nullptr;
	if (CurrentInteractingRobo)
	{
		CurrentInteractingRobo->ShowPickupWidget(false, nullptr);
	}
}

void ALootSpawnerBox::SetSpawnedItem(APickupItem* NewItem)
{
	SpawnedTool = NewItem;
	if (SpawnedTool)
	{
		SpawnedTool->SetOwner(this);
	}
}

void ALootSpawnerBox::ClearSpawnedItem()
{
	SpawnedTool = nullptr;
	if (CurrentInteractingRobo)
	{
		CurrentInteractingRobo->ShowPickupWidget(false, nullptr);
	}
}

void ALootSpawnerBox::OnBoxOpened()
{
	if (!IsValid(CurrentInteractingRobo)) return;
	SpawnLoot();
}

void ALootSpawnerBox::SpawnLoot()
{
	switch (LootType)
	{
	case ELootType::Weapon:
		HandleSpawnWeapon();
		break;
	case ELootType::Tool:
		HandleSpawnTool();
		break;
	}
}

void ALootSpawnerBox::HandleSpawnWeapon()
{
	if (!SpecificWeaponToSpawn.DataTable || SpecificWeaponToSpawn.RowName.IsNone()) return;
	if (IsValid(SpawnedWeapon)) return;

	static const FString ContextString(TEXT("WeaponDataTable Context"));
	FWeaponData* FoundWeaponData = SpecificWeaponToSpawn.DataTable->FindRow<FWeaponData>(SpecificWeaponToSpawn.RowName, ContextString);

	if (FoundWeaponData && FoundWeaponData->WeaponClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnedWeapon = GetWorld()->SpawnActor<AWeapon>(FoundWeaponData->WeaponClass, GetSpawnInFrontOfBox(), FRotator::ZeroRotator, SpawnParams);

		if (IsValid(SpawnedWeapon))
		{
			SpawnedWeapon->RowName = SpecificWeaponToSpawn.RowName;
		}
	}
}

void ALootSpawnerBox::HandleSpawnTool()
{
	if (!ToolPickupClass) return;
	if (IsValid(SpawnedTool)) return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnedTool = GetWorld()->SpawnActor<APickupItem>(ToolPickupClass, GetSpawnInFrontOfBox(), FRotator::ZeroRotator, SpawnParams);
}

FVector ALootSpawnerBox::GetSpawnInFrontOfBox(float ZOffset, float ForwardDist) const
{
	FVector SpawnLocation = GetActorLocation();
	if (IsValid(CurrentInteractingRobo))
	{
		FVector DirectionToRobo = CurrentInteractingRobo->GetActorLocation() - GetActorLocation();
		DirectionToRobo.Z = 0;
		DirectionToRobo.Normalize();
		SpawnLocation += DirectionToRobo * ForwardDist;
	}
	SpawnLocation.Z += ZOffset;
	return SpawnLocation;
}
