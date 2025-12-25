// Source/DTDProject/Weapon/WeaponData.h

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/Texture2D.h"
#include "WeaponData.generated.h"

class AWeapon;

UENUM(BlueprintType)
enum class EWeaponSlot : uint8
{
	Melee,
	Tool,
	Harpoon,
	Gun
};

// 데이터 테이블 구조체
USTRUCT(BlueprintType)
struct FWeaponData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AWeapon> WeaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponSlot Slot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UTexture2D> Icon;

	//사거리
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Range =300.f;

	//초당 발사 수
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FireRate=1.f; 

	//무기별 추가 쿨다운
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CooldownSeconds = 0.0f;

	//연사 가능 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAutomatic = false;

	//몽타주가 끝나기 전에 발사 허용 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAllowFireDuringMontage = true;

	//탄약관련
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxAmmo = 6;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AmmoPerShot = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ReloadTime = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ProjectileSpeed =2000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* AimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> BulletData; //Range 전용
};
