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
	virtual void OnBoxOpened() override;

	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	// 플레이어가 상자 영역을 떠났을 때의 로직을 부모로부터 확장합니다.
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtehrBodyIndex);

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
