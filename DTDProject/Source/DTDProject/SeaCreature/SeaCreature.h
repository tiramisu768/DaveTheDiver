// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SeaCreature.generated.h"

UCLASS()
class DTDPROJECT_API ASeaCreature : public ACharacter
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleAnywhere,Category = "State")
	TObjectPtr<class UFishStateComponent> FishStateComponent;
	UPROPERTY(VisibleAnywhere, Category = "UI")
	TObjectPtr<class UWidgetComponent> FishHPBarWidget;
	UPROPERTY(EditAnywhere, Category = "Spawn", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ADamagePopup> SpawnDamagePopupClass;
	UPROPERTY(EditAnywhere, Category = "Particle")
	TObjectPtr<class UParticleSystem> HitParticle;

public:
	// Sets default values for this character's properties
	ASeaCreature();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void HitBy(float DamageAmount, const FHitResult& HitResult);
	void SpawnDamagePopup(float DamageAmount);
};
