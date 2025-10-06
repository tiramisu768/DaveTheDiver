// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponCategory :uint8
{
	Melee,
	Ranged
};

USTRUCT(BlueprintType)
struct FWeaponData :public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	EWeaponCategory Category;

	UPROPERTY(EditANywhere,BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditANywhere, BlueprintReadWrite)
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Range;

	UPROPERTY(EditANywhere, BlueprintReadWrite)
	float AttackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* AttackMontage;

	UPROPERTY(EditANywhere, BlueprintReadWrite)
	TSubclassOf<AActor> ProjectileClass; //Range Àü¿ë
};

UCLASS()
class DTDPROJECT_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	FName RowName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="Weapon")
	UDataTable* WeaponDataTable;

	FWeaponData* WeaponStats;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	UStaticMeshComponent* MeshComponent;

	virtual void BeginPlay() override;
	virtual void Attack(ACharacter* OwnerCharacter);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
