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

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));

	RootComponent = WeaponMesh;

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

	AMyRobo* Robo = Cast<AMyRobo>(OwnerCharacter);

	switch(WeaponStats->Slot)
	{
		case EWeaponSlot::Melee:
			Robo->AttackTrace(); //실제 대미지를 주는 함수
			break;

		case EWeaponSlot::Harpoon:
		case EWeaponSlot::Gun:
			if (WeaponMesh && WeaponStats->BulletData)
			{
				FVector SpawnLocation = WeaponMesh->GetSocketLocation("Muzzle");
				FRotator SpawnRotation = AimDirection.Rotation();

				FActorSpawnParameters SpawnParam;
				SpawnParam.Owner = GetOwner();
				SpawnParam.Instigator = Cast<APawn>(GetOwner());

				GetWorld()->SpawnActor<AActor>(WeaponStats->BulletData,SpawnLocation, SpawnRotation, SpawnParam);
			}
			break;
	}
	
}

void AWeapon::AdjustSize(float TargetSize)
{
	if (WeaponMesh)
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


