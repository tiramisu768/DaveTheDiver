// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Weapon.h"
#include "GameFrameWork/Character.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
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

void AWeapon::Attack(ACharacter* OwnerCharacter)
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
		GEngine->AddOnScreenDebugMessage(-6, 3.0f, FColor::Yellow, TEXT("Ranged out"));
		if (WeaponStats->ProjectileClass)
		{
			GEngine->AddOnScreenDebugMessage(-8, 3.0f, FColor::Yellow, TEXT("Ranged in"));
			FVector MuzzleLoc = OwnerCharacter->GetActorLocation() + OwnerCharacter->GetActorForwardVector() * 100.f;
			FRotator MuzzleRot = OwnerCharacter->GetActorRotation();

			OwnerCharacter->GetWorld()->SpawnActor<AActor>(WeaponStats->ProjectileClass, MuzzleLoc, MuzzleRot);
		}
	}
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

