// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Engine/DataTable.h"
#include "SeaCreature.generated.h"

USTRUCT(BlueprintType)
struct FSeaCreatureData : public FTableRowBase
{
	GENERATED_BODY()
public:
	FSeaCreatureData() :
		WanderRadius(900.0f),
		SlowRadius(300.0f),
		WanderSpeed(20.0f),
		FleeSpeed(70.0f),
		AttackSpeed(30.0f),
		SeekSpeed(20.0f),
		ReturnSpeed(40.0f),
		Mesh(nullptr) {
	}
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	//float Damage;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	//float HP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	TObjectPtr<class USkeletalMesh> Mesh;






	// 반경
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float WanderRadius; //물고기 배회범위

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float SlowRadius; //물고기 배회범위
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float HomeReturnDist;

	// 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float WanderSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float FleeSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float AttackSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float SeekSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float ReturnSpeed;
};


DECLARE_DELEGATE(FOnAttackMontageEndedDelegate);
UCLASS()
class DTDPROJECT_API ASeaCreature : public ACharacter
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, Category = "Data", meta = (AllowPriaveAccess="true"))
	FName RowName;
	//TArray<FName> StatDataNames;
	UPROPERTY(EditAnywhere, Category = "Data")
	TObjectPtr<class UDataTable> SeaCreatureDataTable;
	FSeaCreatureData* Data;

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
	TObjectPtr<UAnimMontage> DeathFlapMontage;
	FTimerHandle DeathTimerHandle;
	FTimerHandle DeathRotateTimerHandle;
	UPROPERTY(VisibleAnywhere) 
	class USphereComponent* CollectSphere;
	FTimerHandle CollectHintTimer;

public:

	ASeaCreature();

	UPROPERTY(VisibleAnywhere, Category = "AI")
	class USeaCreatureSteeringComponent* SteeringComp;



	// AI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	TObjectPtr<class UBehaviorTree> OverrideBT;

	// 몽타쥬
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	TObjectPtr<UAnimMontage> HitbyMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	TObjectPtr<UAnimMontage> AttackMontage;

	// 성향
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	bool IsAggressive;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UFUNCTION()
	void SpawnSeaCreature();
	void HitBy(float DamageAmount, const FHitResult& HitResult);
	void Die();
	bool isDead();
	void RotateToDeadPose(float DeltaTime);
	void EnableCollectTrigger(bool isEnable);
	UFUNCTION()
	void OnCollectOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void CollectSeaCreature();
	virtual void Attack(class AMyRobo* Target);
	void PostInitializeComponents() override;
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	FOnAttackMontageEndedDelegate OnAttackMontageEndedDelegate;
	void SpawnDamagePopup(float DamageAmount);

};
