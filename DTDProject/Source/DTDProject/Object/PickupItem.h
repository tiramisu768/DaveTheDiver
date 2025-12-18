// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupItem.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class AMyRobo;

UENUM(BlueprintType)
enum class EPickupItemType :uint8
{
	Ammo,
	Tool
};

UCLASS()
class DTDPROJECT_API APickupItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Pickup")
	EPickupItemType ItemType = EPickupItemType::Ammo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category ="Pickup|Ammo")
	int32 AmmoAmount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Pickup|Tool")
	TSubclassOf<AActor> ToolClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Pickup")
	bool AutoPickuponOverlap = true;

protected:
	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<USphereComponent> SphereComp;

	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<UStaticMeshComponent> MeshComp;

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnShpereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable, Category ="Pickup")
	void OnPickedUp(AMyRobo* Robo);
	void OnPickedUP_Implementation(AMyRobo* Robo);
	void FinishPickup();

public:	

	virtual void Tick(float DeltaTime) override;
};
