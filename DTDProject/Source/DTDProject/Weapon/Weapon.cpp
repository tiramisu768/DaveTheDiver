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
			if (WeaponStats->BulletData)
			{
				FVector MuzzleLocation;
				FRotator MuzzleRotation;
				OwnerCharacter->GetActorEyesViewPoint(MuzzleLocation, MuzzleRotation);

				MuzzleLocation += MuzzleRotation.Vector() * 100.0f;

				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = OwnerCharacter;
				SpawnParams.Instigator = OwnerCharacter->GetInstigator();

				GetWorld()->SpawnActor<AActor>(WeaponStats->BulletData, MuzzleLocation, MuzzleRotation, SpawnParams);
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

void AWeapon::Fire(const FVector& AimDirection)
{
	if (!WeaponStats || !WeaponStats->BulletData)
	{
		UE_LOG(LogTemp, Warning, TEXT("Bullet no in %s"), *GetName());
		return;
	}

	UWorld* World = GetWorld();

	UStaticMeshComponent* WeaponMesh = FindComponentByClass<UStaticMeshComponent>();

	if (World && WeaponMesh)
	{
		FVector SpawnLocation = WeaponMesh->GetSocketLocation("Muzzle");
		FRotator SpawnRotation = AimDirection.Rotation();

		FActorSpawnParameters SpawnParam;
		SpawnParam.Owner = GetOwner();
		SpawnParam.Instigator = Cast<APawn>(GetOwner());

		ABullet* SpawnedBullet = World->SpawnActor<ABullet>(WeaponStats->BulletData, SpawnLocation, SpawnRotation, SpawnParam);

		if (SpawnedBullet)
		{
			UE_LOG(LogTemp, Log, TEXT("Fired Bullet: %s"), *SpawnedBullet->GetName());
		}
	}
	else
	{
		if (!WeaponMesh)
		{
			UE_LOG(LogTemp, Warning, TEXT("WeaponMesh No in %s"), *GetName());
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


