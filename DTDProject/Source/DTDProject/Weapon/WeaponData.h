// Source/DTDProject/Weapon/WeaponData.h

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "WeaponData.generated.h"

class AWeapon;

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Melee,
	Ranged
};

// 데이터 테이블 구조체
USTRUCT(BlueprintType)
struct FWeaponData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Range;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> ProjectileClass; //Range 전용
};

// 인벤토리 구조체
USTRUCT(BlueprintType)
struct FWeaponTypeInventory
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	EWeaponType WeaponType;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	TArray<TObjectPtr<AWeapon>> Weapons;

	UPROPERTY()
	int32 CurrentIndex = 0;
};