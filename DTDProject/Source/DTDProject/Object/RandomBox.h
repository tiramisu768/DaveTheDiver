// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractionObject.h"
#include "RandomBox.generated.h"

class UBoxComponent;
class AWeapon;
class UDataTable;

UENUM(BlueprintType)
enum class EBoxType :uint8
{
	Weapon UMETA(DisplayName = "Weapon"),
	Ammo UMETA(DisplayName = "Ammo"),
	Oxygen UMETA(DisplayName = "Oxygen"),
	Tool UMETA(DisplayName = "Tool"),
};

UCLASS()
class DTDPROJECT_API ARandomBox : public AActor,public IInteractionObject
{
	GENERATED_BODY()

public:
	ARandomBox();
	void ClearSpawnedWeapon();
	void SetSpawnedWeapon(AWeapon* NewWeapon);

	virtual void Interact(AMyRobo* InteractingRobo) override;
	virtual void ShowInteractionWidget(bool bShow) override;
	virtual void StartFocus() override;
	virtual void EndFocus() override;

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;


	UFUNCTION()
	void RandomBoxOnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void RandomBoxOnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	void UpdateOpenAnimation(float DeltaTime); //밝기 애니메이션 실행

	void SpawnLoot();
	void HandleSpawnWeapon();
	void HandleSpawnAmmo();
	void HandleSpawnOxygen();
	void HandleSpawnTool();

	FVector GetSpawnInFrontOfBox(float ZOffset = 100.f, float ForwardDist = 60.f) const;


	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> BoxFrameMesh;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<class UBoxComponent> BoxCollision;

	UPROPERTY()
	UMaterialInstanceDynamic* DynMat;

	UPROPERTY()
	bool IsOpen{ false };
	UPROPERTY()
	bool IsRoboOverlap{ false };
	UPROPERTY()
	bool IsOpening{ false }; //오픈 애님 진행 여부

	UPROPERTY()
	TObjectPtr<AMyRobo> CurrentInteractingRobo;

	UPROPERTY()
	TObjectPtr<AWeapon> SpawnedWeapon;

	UPROPERTY()
	TObjectPtr<AActor> SpawnedItemActor; //Ammo, Tool 픽업을 위해

	UPROPERTY(EditAnywhere, Category = "RandomBox")
	EBoxType BoxType = EBoxType::Weapon;

	UPROPERTY(EditAnywhere, Category = "RandomBox|Weapon")
	TObjectPtr<UDataTable> WeaponDataTable;

	UPROPERTY(EditAnywhere, Category = "RandomBox|Ammo")
	TSubclassOf<AActor> AmmoPickupClass;

	UPROPERTY(EditAnywhere, Category = "RandomBox|Tool")
	TSubclassOf<AActor> ToolPickupClass;

	UPROPERTY(EditAnywhere, Category = "RandomBox|Oxygen")
	float OxygenRefillAmount = 0.f;

	UPROPERTY(EditAnywhere, Category = "RandomBox|Visual")
	float Brightness = 1.0f;

	UPROPERTY(EditAnywhere, Category = "RandomBox|Visual")
	float OpenSpeed = 2.0f;
};
