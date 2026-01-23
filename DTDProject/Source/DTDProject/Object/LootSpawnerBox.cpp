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

void ALootSpawnerBox::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (AMyRobo* Robo = Cast<AMyRobo>(OtherActor))
	{
		// 상자가 이미 열려있고, 스폰된 아이템이 있다면 획득 가능하도록 설정합니다.
		if (IsOpen)
		{
			if (IsValid(SpawnedWeapon))
			{
				Robo->SetAcquirableActor(SpawnedWeapon);
			}
			else if (IsValid(SpawnedTool))
			{
				Robo->SetAcquirableActor(SpawnedTool);
			}
		}
	}
}

void ALootSpawnerBox::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	if (AMyRobo* Robo = Cast<AMyRobo>(OtherActor))
	{
		// 로봇이 영역을 떠나면 획득 가능한 아이템 설정을 모두 해제합니다.
		Robo->SetAcquirableActor(nullptr);
	}
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
	if (!WeaponDataTable || WeaponDataTable->GetRowMap().Num() == 0) return;
	if (IsValid(SpawnedWeapon)) return;

	TArray<FName> RowNames = WeaponDataTable->GetRowNames();
	const FName RandomRowName = RowNames[FMath::RandRange(0, RowNames.Num() - 1)];
	static const FString ContextString(TEXT("WeaponDataTable Context"));
	FWeaponData* FoundWeaponData = WeaponDataTable->FindRow<FWeaponData>(RandomRowName, ContextString);

	if (FoundWeaponData && FoundWeaponData->WeaponClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnedWeapon = GetWorld()->SpawnActor<AWeapon>(FoundWeaponData->WeaponClass, GetSpawnInFrontOfBox(), FRotator::ZeroRotator, SpawnParams);

		if (IsValid(SpawnedWeapon))
		{
			SpawnedWeapon->RowName = RandomRowName;
			SpawnedWeapon->PostInitializeComponents();
			if (IsValid(CurrentInteractingRobo))
			{
				CurrentInteractingRobo->SetAcquirableActor(SpawnedWeapon);
			}
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

	if (IsValid(SpawnedTool) && IsValid(CurrentInteractingRobo))
	{
		CurrentInteractingRobo->SetAcquirableActor(SpawnedTool);
	}
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
