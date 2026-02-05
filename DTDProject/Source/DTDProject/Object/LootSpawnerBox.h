// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object/BaseBox.h"
#include "LootSpawnerBox.generated.h"

class AWeapon;
class APickupItem;
class UDataTable;

UENUM(BlueprintType)
enum class ELootType :uint8
{
	Weapon UMETA(DisplayName = "Weapon"),
	Tool UMETA(DisplayName ="Tool"),
};

UCLASS()
class DTDPROJECT_API ALootSpawnerBox : public ABaseBox
{
	GENERATED_BODY()
	
public:	
	ALootSpawnerBox();

	void SetSpawnedWeapon(AWeapon* NewWeapon);
	void ClearSpawnedWeapon();

	void SetSpawnedItem(APickupItem* NewItem);
	void ClearSpawnedItem();

protected:
	UPROPERTY(EditAnywhere, Category = "Loot Settings", meta = (EditCondition = "LootType == ELootType::Weapon"))
	FDataTableRowHandle SpecificWeaponToSpawn;

	virtual void OnBoxOpened() override;

private:
	void SpawnLoot();
	void HandleSpawnWeapon();
	void HandleSpawnTool();

	FVector GetSpawnInFrontOfBox(float ZOffset = 100.f, float ForwardDist = 60.f) const;

	UPROPERTY(EditAnywhere, Category ="Loot")
	ELootType LootType = ELootType::Weapon;

	UPROPERTY()
	TObjectPtr<AWeapon> SpawnedWeapon;

	UPROPERTY()
	TObjectPtr<APickupItem> SpawnedTool;

	UPROPERTY(EditAnywhere,Category ="Loot|Weapon")
	TObjectPtr<UDataTable> WeaponDataTable;

	UPROPERTY(EditAnywhere, Category ="Loot|Tool")
	TSubclassOf<APickupItem> ToolPickupClass;

};
