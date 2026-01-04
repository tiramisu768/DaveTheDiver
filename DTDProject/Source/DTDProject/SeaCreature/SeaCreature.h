// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Engine/DataTable.h"
#include "Interface/AttackTraceNotify/AttackTraceNotify.h"
#include "SeaCreature.generated.h"

UENUM(BlueprintType)
enum class ESeaDisposition :uint8
{
	Passive UMETA(DisplayName = "Passive"),
	Aggressive UMETA(DisplayName = "Aggressive"),
	Monster UMETA(DisplayName = "Monster")
};

USTRUCT(BlueprintType)
struct FSeaCreatureData : public FTableRowBase
{
	GENERATED_BODY()
public:
	FSeaCreatureData();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1. Default Info")
	FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1. Default Info")
	int32 Grade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1. Default Info")
	float Weight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1. Default Info")
	float MaxHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1. Default Info")
	ESeaDisposition Disposition = ESeaDisposition::Passive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "2. Visual")
	TSoftObjectPtr<UTexture2D> FishIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "2. Visual")
	TObjectPtr<class USkeletalMesh> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "2. Visual")
	TSubclassOf<UAnimInstance> AnimClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "2. Visual")
	TObjectPtr<UAnimMontage> HitbyMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "4. Common")
	float WanderRadius = { 900.0f }; //물고기 배회범위

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "4. Common")
	float SlowRadius = { 300.0f }; //물고기 배회범위

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "4. Common")
	float SightRadius = 1500.0f; // 로보 인지 범위

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "4. Common")
	float ActionTriggerDistance = 400.0f; //물고기 추격/도망 시작 범위

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "4. Common")
	float SafeDistance = 1000.f; //물고기 추격/도망 중단 범위

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "4. Common")
	float WanderSpeed = { 5.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "4. Common")
	float ReturnSpeed = { 5.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "4. Common")
	float Acceleration = { 1500.f }; //가속도

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "5. Passive")
	float FleeSpeed = { 20.0f };


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "6. Aggressive")
	TObjectPtr<UAnimMontage> AttackMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "6. Aggressive")
	float AttackRange = 50.0f; //물고기 공격 시작 범위
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "6. Aggressive")
	float ChaseSpeed = { 10.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "6. Aggressive")
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FDataTableRowHandle SeaCreatureDataHandle;

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	float HomeReturnDist=0.f;

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
