// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Weapon/WeaponData.h"
#include "Weapon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnWeaponStateChanged, AWeapon*, Sender, int32, CurrentAmmo, float, CooldownRatio);

UCLASS()
class DTDPROJECT_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeapon();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	FName RowName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="Weapon")
	UDataTable* WeaponDataTable;

	UPROPERTY(BlueprintAssignable, Category="Weapon")
	FOnWeaponStateChanged OnGunAmmoUpdated;

	void StartFire(ACharacter* OwnerCharacter, const FVector& FireDirection);
	void StopFire(ACharacter* OwnerCharacter);
	void PlayFireMontage(ACharacter* OwnerCharacter);
	void StopFireMontage(ACharacter* OwnerCharacter);
	void SpawnProjectileAtMuzzle(const FVector& Direction);
	virtual void TryFire(ACharacter* OwnerCharacter, const FVector& FireDirection = FVector::ZeroVector);
	void ReloadToMax();

	EWeaponSlot GetSlotType() const;
	const FWeaponData* GetWeaponStats() const { return WeaponStats; }
	FVector GetMuzzleLocation() const;
	int32 GetCurrentAmmo() const { return CurrentAmmo; }

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	FWeaponData* WeaponStats;
	FTimerHandle CooldownTimerHandle;
	FTimerHandle FireTimerHandle;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Weapon", meta=(AllowPrivateAccess="true"))
	int32 CurrentAmmo = 0;

	bool bIsFiring = false;
	FVector CurrentFireDirection;

	bool ConsumeAmmo(int32 Amount = 1);
	bool CanFire() const;
	void StartCooldown();
	void OnCooldownExpired();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* ParentMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* WeaponMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	TSubclassOf<class ABullet> BulletClass;
};
