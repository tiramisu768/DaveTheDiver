// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SeaCreature.generated.h"

UENUM(BlueprintType)
enum class ESeaDisposition : uint8{Passive, Aggressive};

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
	UPROPERTY(EditAnywhere, Category = "FX")
	TObjectPtr<class UNiagaraSystem> HitEffect;
	FVector Forward = { 0.1f,0.0f,0.0f };
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAnimMontage> HitbyMontage;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAnimMontage> DeathFlapMontage;
	FTimerHandle DeathTimerHandle;
	FTimerHandle DeathRotateTimerHandle;
	UPROPERTY(VisibleAnywhere) 
	class USphereComponent* CollectSphere;
	FTimerHandle CollectHintTimer;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAnimMontage> AttackMontage;

public:
	// Sets default values for this character's properties
	ASeaCreature();
	//DataTable
	/*UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "AI")
	FName SpeciesId;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category ="AI")
	class UDataTable* SpeciesTable = nullptr;*/

	UPROPERTY(VisibleAnywhere, Category = "AI")
	class USeaCreatureSteeringComponent* SteeringComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	ESeaDisposition Disposition = ESeaDisposition::Aggressive;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	TObjectPtr<class UBehaviorTree> OverrideBT = nullptr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void HitBy(float DamageAmount, const FHitResult& HitResult);
	void Die();
	bool isDead();
	void RotateToDeadPose(float DeltaTime);
	void EnableCollectTrigger(bool isEnable);
	UFUNCTION()
	void OnCollectOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void CollectSeaCreature();
	virtual void Attack(class AMyRobo* Target);
	void SpawnDamagePopup(float DamageAmount);
};
