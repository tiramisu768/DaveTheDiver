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

	PlayFireMontage(OwnerCharacter);

	if (WeaponStats->bAutomatic && WeaponStats->FireRate > 0.f)
	{
		const float Interval = 1.f / WeaponStats->FireRate;
		if (GetWorld())
		{
			GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, [this, OwnerCharacter]() {
				this->TryFire(OwnerCharacter, this->CurrentFireDirection);
				},Interval,true);
		}
	}
}

void AWeapon::StopFire(ACharacter* OwnerCharacter)
{
	bIsFiring = false;
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);
	}
	StopFireMontage(OwnerCharacter);
}

void AWeapon::PlayFireMontage(ACharacter* OwnerCharacter)
{
	if (!OwnerCharacter || !WeaponStats) return;

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

void AWeapon::StopFireMontage(ACharacter* OwnerCharacter)
{
	if (!OwnerCharacter || !WeaponStats) return;

	if (WeaponStats->AttackMontage)
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

void AWeapon::SpawnProjectileAtMuzzle(const FVector& Direction)
{
	if (!WeaponStats)
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnProjectileAtMuzzle: WeaponStats null on %s"), *GetName());
		return;
	}

	// 발사 가능 여부(쿨다운/탄약 등)
	if (!CanFire())
	{
		UE_LOG(LogTemp, Verbose, TEXT("SpawnProjectileAtMuzzle: CanFire() false on %s"), *GetName());
		return;
	}

	// 탄약 소모 (AnimNotify 경로에서도 탄약을 소비해야 함)
	if (!ConsumeAmmo(WeaponStats->AmmoPerShot))
	{
		UE_LOG(LogTemp, Verbose, TEXT("SpawnProjectileAtMuzzle: ConsumeAmmo failed on %s"), *GetName());
		return;
	}

	// 실제 스폰
	if (!WeaponStats->BulletData)
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnProjectileAtMuzzle: BulletData null for weapon Row '%s'"), *RowName.ToString());
		StartCooldown();
		return;
	}

	UWorld* World = GetWorld();
	if (!World) return;

	FVector SpawnLocation = GetMuzzleLocation();
	FRotator SpawnRotation = Direction.Rotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();
	SpawnParams.Instigator = Cast<APawn>(GetOwner());
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ABullet* SpawnedBullet = World->SpawnActor<ABullet>(WeaponStats->BulletData, SpawnLocation, SpawnRotation, SpawnParams);

	if (SpawnedBullet)
	{
		if (WeaponStats->ProjectileSpeed > 0.f)
		{
			const float LifeSpan = WeaponStats->Range / WeaponStats->ProjectileSpeed;
			SpawnedBullet->SetLifeSpan(LifeSpan);
		}

		if (UProjectileMovementComponent* ProjMove = SpawnedBullet->FindComponentByClass<UProjectileMovementComponent>())
		{
			ProjMove->InitialSpeed = WeaponStats->ProjectileSpeed;
			ProjMove->MaxSpeed = WeaponStats->ProjectileSpeed;
			SpawnedBullet->Damage = WeaponStats->Damage;
		}
	}
	StartCooldown();
}

void AWeapon::TryFire(ACharacter* OwnerCharacter, const FVector& FireDirection)
{
	if (!WeaponStats || !OwnerCharacter) return;

	// 몽타주가 끝나기 전 발사 금지 옵션
	if (!WeaponStats->bAllowFireDuringMontage)
	{
		if (USkeletalMeshComponent* Mesh = OwnerCharacter->GetMesh())
		{
			if (UAnimInstance* Anim = Mesh->GetAnimInstance())
			{
				if ((WeaponStats->AttackMontage && Anim->Montage_IsPlaying(WeaponStats->AttackMontage)) || (WeaponStats->AimMontage && Anim->Montage_IsPlaying(WeaponStats->AimMontage)))
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

	SpawnProjectileAtMuzzle(FireDirection);

	StartCooldown();
}

void AWeapon::ReloadToMax()
{
	if (WeaponStats)
	{
		CurrentAmmo = WeaponStats->MaxAmmo;
		OnGunAmmoUpdated.Broadcast(this, CurrentAmmo, 0.0f);
	}
}


bool AWeapon::ConsumeAmmo(int32 Amount)
{
	if (!WeaponStats) return false;
	if (WeaponStats->MaxAmmo <= 0) return true;
	if (CurrentAmmo >= Amount)
	{
		CurrentAmmo -= Amount;
		OnGunAmmoUpdated.Broadcast(this,CurrentAmmo, 0.0f);
		UE_LOG(LogTemp, Warning, TEXT("ConsumeAmmo Start1 %d"), CurrentAmmo);
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
		OnGunAmmoUpdated.Broadcast(this, CurrentAmmo, 1.0f);
	}
}

void AWeapon::OnCooldownExpired()
{
	OnGunAmmoUpdated.Broadcast(this,CurrentAmmo, 0.0f);
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


