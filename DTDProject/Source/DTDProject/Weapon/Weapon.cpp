// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Weapon.h"
#include "MyRobo/MyRobo.h"
#include "GameFrameWork/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	if (WeaponDataTable)
	{
		WeaponStats = WeaponDataTable->FindRow<FWeaponData>(RowName, TEXT(""));
	}
}

EWeaponSlot AWeapon::GetSlotType() const
{
	if (WeaponStats)
	{
		return WeaponStats->Slot;
	}

	return EWeaponSlot::Melee;
}

void AWeapon::Attack(ACharacter* OwnerCharacter)
{
	if (!WeaponStats || !OwnerCharacter) return;

	AMyRobo* Robo = Cast<AMyRobo>(OwnerCharacter);

	switch(WeaponStats->Slot)
	{
		case EWeaponSlot::Melee:
			Robo->AttackTrace(); //실제 대미지를 주는 함수
			break;

		case EWeaponSlot::Harpoon:
		case EWeaponSlot::Gun:
			if (WeaponStats->ProjectileClass)
			{
				FVector MuzzleLocation;
				FRotator MuzzleRotation;
				OwnerCharacter->GetActorEyesViewPoint(MuzzleLocation, MuzzleRotation);

				MuzzleLocation += MuzzleRotation.Vector() * 100.0f;

				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = OwnerCharacter;
				SpawnParams.Instigator = OwnerCharacter->GetInstigator();

				GetWorld()->SpawnActor<AActor>(WeaponStats->ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
			}
			break;
	}
	
}

void AWeapon::AdjustSize(float TargetSize)
{
	if (UStaticMeshComponent* WeaponMesh = FindComponentByClass<UStaticMeshComponent>())
	{
		if (UStaticMesh* MeshAsset = WeaponMesh->GetStaticMesh())
		{
			FVector OriginalSize = MeshAsset->GetBounds().GetBox().GetSize();
			float MaxOriginalSize = FMath::Max3(OriginalSize.X, OriginalSize.Y, OriginalSize.Z);

			if (MaxOriginalSize > KINDA_SMALL_NUMBER)
			{
				float ScaleMultiplier = TargetSize / MaxOriginalSize;
				SetActorRelativeScale3D(FVector(ScaleMultiplier));
			}
		}
	}
}

FWeaponData AWeapon::GetWeaponStatsCopy() const
{
	if (WeaponStats)
	{
		return *WeaponStats;
	}
	return FWeaponData();
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

