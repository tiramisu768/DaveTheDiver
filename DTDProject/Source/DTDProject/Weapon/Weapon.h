// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Weapon/WeaponData.h"
#include "Weapon.generated.h"


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

	UFUNCTION(BlueprintPure, Category = "Weapon")
	EWeaponSlot GetSlotType() const;

	const FWeaponData* GetWeaponStats() const { return WeaponStats; }

	UFUNCTION(BlueprintPure, Category = "Weapon", meta = (DisplayName = "GetWeaponStats (Copy)"))
	FWeaponData GetWeaponStatsCopy() const;

	FVector GetMuzzleLocation() const;

	virtual void Tick(float DeltaTime) override;

	virtual void Attack(ACharacter* OwnerCharacter, const FVector& FireDirection = FVector::ZeroVector);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* ParentMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* WeaponMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	TSubclassOf<class ABullet> BulletClass;

	virtual void BeginPlay() override;

private:
	FWeaponData* WeaponStats;
};
