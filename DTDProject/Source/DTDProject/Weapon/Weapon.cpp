// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Weapon.h"
#include "Weapon/Bullet.h"
#include "MyRobo/MyRobo.h"
#include "GameFrameWork/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

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

	if (WeaponStats)
	{
		CurrentAmmo = WeaponStats->MaxAmmo;
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

void AWeapon::Attack(ACharacter* OwnerCharacter, const FVector& FireDirection)
{
	if (!WeaponStats || !OwnerCharacter) return;

	if (!WeaponStats->bAllowFireDuringMontage)
	{
		if (USkeletalMeshComponent* Mesh = OwnerCharacter->GetMesh())
		{
			if (UAnimInstance* Anim = Mesh->GetAnimInstance())
			{
				if ((WeaponStats->AttackMontage && Anim->Montage_Play(WeaponStats->AttackMontage)) || (WeaponStats->AimMontage && Anim->Montage_Play(WeaponStats->AimMontage)))
				{
					return;
				}
			}
		}
	}

	if (!CanFire()) return;

	if (!ConsumeAmmo(WeaponStats->AmmoPerShot))
	{
		//탄약 부족 처리(사운드 등)
		return;
	}

	switch (WeaponStats->Slot)
	{
		case EWeaponSlot::Melee:
			break;

		case EWeaponSlot::Harpoon:
		case EWeaponSlot::Gun:
		{
			if (WeaponMesh && WeaponStats->BulletData)
			{
				UWorld* World = GetWorld();
				if (!World) return;

				FVector SpawnLocation = GetMuzzleLocation();
				FRotator SpawnRotation = FireDirection.Rotation();

				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = GetOwner();
				SpawnParams.Instigator = Cast<APawn>(GetOwner());
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				ABullet* SpawnedBullet = World->SpawnActor<ABullet>(WeaponStats->BulletData, SpawnLocation, SpawnRotation, SpawnParams);

				if (SpawnedBullet)
				{
					if (UProjectileMovementComponent* ProjMove = SpawnedBullet->FindComponentByClass<UProjectileMovementComponent>())
					{
						ProjMove->InitialSpeed = WeaponStats->ProjectileSpeed;
						ProjMove->MaxSpeed = WeaponStats->ProjectileSpeed;
						SpawnedBullet->Damage = WeaponStats->Damage;
					}
				}
			}
			break;
		}
	}
	StartCooldown();
}

bool AWeapon::ConsumeAmmo(int32 Amount)
{
	if (!WeaponStats) return false;
	if (WeaponStats->MaxAmmo <= 0) return true;
	if (CurrentAmmo >= Amount)
	{
		CurrentAmmo -= Amount;
		OnWeaponStateChanged.Broadcast(CurrentAmmo, 0.0f);
		return true;
	}
	return false;
}

bool AWeapon::CanFire() const
{
	if (!WeaponStats) return false;

	if (!WeaponStats->bAutomatic && WeaponStats->MaxAmmo > 0 && CurrentAmmo <= 0) return false;
	if (GetWorld() && GetWorld()->GetTimerManager().IsTimerActive(CooldownTimerHandle)) return false;
	return true;
}

void AWeapon::StartCooldown()
{
	if (!WeaponStats) return;

	float Cool = WeaponStats->CooldownSeconds;

	if (WeaponStats->bAutomatic && WeaponStats->FireRate > 0.f)
	{
		Cool = 1.f / WeaponStats->FireRate;
	}

	if (Cool <= 0.f) return;

	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle, this, &AWeapon::OnCooldownExpired, Cool, false);
		OnWeaponStateChanged.Broadcast(CurrentAmmo, 1.0f);
	}
}

void AWeapon::OnCooldownExpired()
{
	OnWeaponStateChanged.Broadcast(CurrentAmmo, 0.0f);
}

FWeaponData AWeapon::GetWeaponStatsCopy() const
{
	if (WeaponStats)
	{
		return *WeaponStats;
	}
	return FWeaponData();
}

FVector AWeapon::GetMuzzleLocation() const
{
	if (WeaponMesh && WeaponMesh->DoesSocketExist(TEXT("Muzzle")))
	{
		return WeaponMesh->GetSocketLocation(TEXT("Muzzle"));
	}

	UE_LOG(LogTemp, Warning, TEXT("Weapon '%s' is No Muzzle socket."), *GetName());
	return GetActorLocation();
}

void AWeapon::StartFire(ACharacter* OwnerCharacter, const FVector& FireDirection)
{
	if (!OwnerCharacter || !WeaponStats) return;

	bIsFiring = true;
	CurrentFireDirection = FireDirection;

	if (WeaponStats->AttackMontage)
	{
		if (USkeletalMeshComponent* Mesh = OwnerCharacter->GetMesh())
		{
			if (UAnimInstance* Anim = Mesh->GetAnimInstance())
			{
				if (!Anim->Montage_IsPlaying(WeaponStats->AttackMontage))
				{
					Anim->Montage_Play(WeaponStats->AttackMontage);
				}
			}
		}
	}
}

void AWeapon::StopFire(ACharacter* OwnerCharacter)
{
	bIsFiring = false;

	if (WeaponStats && WeaponStats->AttackMontage)
	{
		if (OwnerCharacter)
		{
			if (USkeletalMeshComponent* Mesh = OwnerCharacter->GetMesh())
			{
				if (UAnimInstance* Anim = Mesh->GetAnimInstance())
				{
					if (Anim->Montage_IsPlaying(WeaponStats->AttackMontage))
					{
						Anim->Montage_Stop(0.2f, WeaponStats->AttackMontage);
					}
				}
			}
		}
	}
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


