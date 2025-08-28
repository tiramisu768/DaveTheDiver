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
	FVector Forward = { 0.1f,0.0f,0.0f };
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAnimMontage> HitbyMontage;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAnimMontage> DeathMontage;
	FTimerHandle DeathTimerHandle;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAnimMontage> AttackMontage;

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
	bool isDead();
	virtual void Attack(class AMyRobo* Target);
	void SpawnDamagePopup(float DamageAmount);
};
