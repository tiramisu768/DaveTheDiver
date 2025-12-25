// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Weapon/WeaponData.h"
#include "Weapon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWeaponStateChanged, int32, CurrentAmmo, float, CooldownRatio);

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
	FOnWeaponStateChanged OnWeaponStateChanged;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Weapon")
	int32 CurrentAmmo = 0;

	FTimerHandle CooldownTimerHandle;

	UFUNCTION(BlueprintPure, Category = "Weapon")
	EWeaponSlot GetSlotType() const;

	const FWeaponData* GetWeaponStats() const { return WeaponStats; }

	UFUNCTION(BlueprintPure, Category = "Weapon", meta = (DisplayName = "GetWeaponStats (Copy)"))
	FWeaponData GetWeaponStatsCopy() const;

	FVector GetMuzzleLocation() const;

	void StartFire(ACharacter* OwnerCharacter, const FVector& FireDirection);
	void StopFire(ACharacter* OwnerCharacter);
	virtual void Attack(ACharacter* OwnerCharacter, const FVector& FireDirection = FVector::ZeroVector);

	bool ConsumeAmmo(int32 Amount = 1);
	bool CanFire() const;
	void StartCooldown();
	void OnCooldownExpired();

protected:
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* ParentMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* WeaponMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	TSubclassOf<class ABullet> BulletClass;

	FWeaponData* WeaponStats;

	bool bIsFiring = false;
	FVector CurrentFireDirection;
};
