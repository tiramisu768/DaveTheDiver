// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Engine/DataTable.h"
#include "Interface/AttackTraceNotify/AttackTraceNotify.h"
#include "SeaCreature.generated.h"

USTRUCT(BlueprintType)
struct FSeaCreatureData : public FTableRowBase
{
	GENERATED_BODY()
public:
	FSeaCreatureData();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1. 기본 정보")
	FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1. 기본 정보")
	int32 Grade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1. 기본 정보")
	float Weight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1. 기본 정보")
	float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "2. 시각적 요소")
	TSoftObjectPtr<UTexture2D> FishIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "2. 시각적 요소")
	TObjectPtr<class USkeletalMesh> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "2. 시각적 요소")
	TSubclassOf<UAnimInstance> AnimClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "2. 시각적 요소")
	TObjectPtr<UAnimMontage> HitbyMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "3. 행동 성향")
	bool IsAggressive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "4. 공통 행동")
	float WanderRadius = { 900.0f }; //물고기 배회범위

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "4. 공통 행동")
	float SlowRadius = { 300.0f }; //물고기 배회범위

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "4. 공통 행동")
	float SightRadius = 1500.0f; // 로보 인지 범위

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "4. 공통 행동")
	float ActionTriggerDistance = 400.0f; //물고기 추격/도망 시작 범위

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "4. 공통 행동")
	float SafeDistance = 1000.f; //물고기 추격/도망 중단 범위

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "4. 공통 행동")
	float WanderSpeed = { 5.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "4. 공통 행동")
	float ReturnSpeed = { 5.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "4. 공통 행동")
	float Acceleration = { 1500.f }; //가속도

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "5. 비공격형 행동", meta = (EditCondition = "!IsAggressive"))
	float FleeSpeed = { 20.0f };


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "IsAggressive"))
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "6. 공격형 행동", meta = (EditCondition = "IsAggressive"))
	TObjectPtr<UAnimMontage> AttackMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "6. 공격형 행동", meta = (EditCondition = "IsAggressive"))
	float AttackRange = 50.0f; //물고기 공격 시작 범위
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "6. 공격형 행동", meta = (EditCondition = "IsAggressive"))
	float ChaseSpeed = { 10.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "6. 공격형 행동", meta = (EditCondition = "IsAggressive"))
	float AttackSpeed{ 10.0f };


};


class USkeletalMeshComponent;
class UFloatingPawnMovement;
class UFishStateComponent;
class UWidgetComponent;
class USphereComponent;
class UDataTable;
class UAnimMontage;

DECLARE_DELEGATE(FOnAttackMontageEndedDelegate);
UCLASS()
class DTDPROJECT_API ASeaCreature : public APawn,public IAttackTraceNotify
{
	GENERATED_BODY()

public:
	ASeaCreature();

	FOnAttackMontageEndedDelegate OnAttackMontageEndedDelegate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USphereComponent> SphereComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USkeletalMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UFloatingPawnMovement> MovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UFishStateComponent> FishStateComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USeaCreatureSteeringComponent* SteeringComp;

	USkeletalMeshComponent* GetMesh() const { return Mesh.Get(); }

	const FSeaCreatureData* GetData() const { return Data; }

	float GetHomeReturnDist() const { return HomeReturnDist; }

	virtual void Tick(float DeltaTime) override;

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

	virtual void Attack(class AMyRobo* Target);

	virtual void AttackTrace() override;

	void PostInitializeComponents() override;

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	void SpawnDamagePopup(float DamageAmount);

	void StopAIBehavior();

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UPROPERTY(EditAnywhere, Category = "Data", meta = (AllowPriaveAccess="true"))
	FName RowName;

	float HomeReturnDist=0.f;

	//TArray<FName> StatDataNames;

	UPROPERTY(EditAnywhere, Category = "Data")
	TObjectPtr<class UDataTable> SeaCreatureDataTable;

	FSeaCreatureData* Data;

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
};
