// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Weapon.h"
#include "GameFrameWork/Character.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	if (WeaponDataTable && RowName != NAME_None)
	{
		WeaponStats = WeaponDataTable->FindRow<FWeaponData>(RowName, TEXT("Dagger"));
		if (WeaponStats)
		{
			UE_LOG(LogTemp, Log, TEXT("Loaded Weapon:%s, Damage:%f"), *WeaponStats->Name, WeaponStats->Damage);               
		}
	}
}

void AWeapon::Attack(ACharacter* OwnerCharacter, const FVector& AimDir /*= FVector::ZeroVector*/)
{
	if (!WeaponStats) return;

	if (WeaponStats->Category == EWeaponCategory::Melee)
	{
		if (WeaponStats->AttackMontage)
		{
			OwnerCharacter->PlayAnimMontage(WeaponStats->AttackMontage);
		}
	}
	else if (WeaponStats->Category == EWeaponCategory::Ranged)
	{
		if (WeaponStats->ProjectileClass)
		{
			FVector MuzzleLoc = OwnerCharacter->GetActorLocation() + OwnerCharacter->GetActorForwardVector() * 100.f;
			FRotator MuzzleRot = OwnerCharacter->GetActorRotation();

			// If AimDir provided, rotate to that direction
			if (!AimDir.IsNearlyZero())
			{
				MuzzleRot = AimDir.Rotation();
			}

			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = OwnerCharacter;
			SpawnParams.Instigator = OwnerCharacter->GetInstigator();

			GetWorld()->SpawnActor<AActor>(WeaponStats->ProjectileClass, MuzzleLoc, MuzzleRot, SpawnParams);
		}
	}
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

