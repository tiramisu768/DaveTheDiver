// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Weapon.h"
#include "MyRobo/MyRobo.h"
#include "GameFrameWork/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "Weapon/Bullet.h"

// Sets default values
AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	ParentMesh = CreateDefaultSubobject<USceneComponent>(TEXT("ParentMesh"));

	RootComponent = ParentMesh;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));

	WeaponMesh->SetupAttachment(RootComponent);

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

void AWeapon::Attack(ACharacter* OwnerCharacter, const FVector& AimDirection)
{
	if (!WeaponStats || !OwnerCharacter) return;

	switch(WeaponStats->Slot)
	{
		case EWeaponSlot::Melee:
			if(AMyRobo* Robo = Cast<AMyRobo>(OwnerCharacter))
			{
				Robo->AttackTrace(); //근접 공격은 트레이스로 판정
			}
			break;

		case EWeaponSlot::Harpoon:
		case EWeaponSlot::Gun:
			if (WeaponMesh && WeaponStats->BulletData)
			{
				UWorld* World = GetWorld();
				if (!World) return;

				FVector SpawnLocation = WeaponMesh->GetSocketLocation("Muzzle");
				FRotator SpawnRotation = AimDirection.Rotation();

				FActorSpawnParameters SpawnParam;
				SpawnParam.Owner = GetOwner();
				SpawnParam.Instigator = Cast<APawn>(GetOwner());
				SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				ABullet* SpawnedBullet = World->SpawnActor<ABullet>(WeaponStats->BulletData,SpawnLocation, SpawnRotation, SpawnParam);

				if (SpawnedBullet)
				{
					//const FVector NewScale(0.0003f);
					const FVector NewScale(0.03f);
					SpawnedBullet->SetActorScale3D(NewScale);
				}
			}
			break;
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


