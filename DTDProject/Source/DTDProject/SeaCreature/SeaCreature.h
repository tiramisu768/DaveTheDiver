// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
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
		WanderSpeed(5.0f),
		FleeSpeed(20.0f),
		AttackSpeed(10.0f),
		SeekSpeed(10.0f),
		ReturnSpeed(5.0f),
		Mesh(nullptr) {
	}
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	//float Damage;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	//float HP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 Grade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float Weight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	TObjectPtr<class USkeletalMesh> Mesh;

	// 몽타쥬
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	TObjectPtr<UAnimMontage> HitbyMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	TObjectPtr<UAnimMontage> AttackMontage;

	// 성향
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	bool IsAggressive;



	// 반경
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float WanderRadius; //물고기 배회범위

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float SlowRadius; //물고기 배회범위

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


class UCapsuleComponent;
class USkeletalMeshComponent;
class UFloatingPawnMovement;
class UFishStateComponent;
class UWidgetComponent;
class USphereComponent;
class UDataTable;
class UAnimMontage;

DECLARE_DELEGATE(FOnAttackMontageEndedDelegate);
UCLASS()
class DTDPROJECT_API ASeaCreature : public APawn
{
	GENERATED_BODY()
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



public:

	ASeaCreature();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BT")
	TObjectPtr<class UBehaviorTree> OverrideBT;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCapsuleComponent> CapsuleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USkeletalMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UFloatingPawnMovement> MovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UFishStateComponent> FishStateComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USeaCreatureSteeringComponent* SteeringComp;

	USkeletalMeshComponent* GetMesh() const { return Mesh.Get(); }

	UCapsuleComponent* GetCapsuleComponent() const { return CapsuleComponent.Get(); }

	const FSeaCreatureData* GetData() const { return Data; }

	float GetHomeReturnDist() const { return HomeReturnDist; }

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	void SpawnSeaCreature();

	void HitBy(float DamageAmount, const FHitResult& HitResult);

	void Die();

	bool isDead();

	void RotateToDeadPose(float DeltaTime);

	void EnableCollectTrigger(bool isEnable);

	UFUNCTION()
	void OnCollectOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	void CollectSeaCreature(AActor* OtherActor);

	virtual void Attack(class AMyRobo* Target);

	void PostInitializeComponents() override;

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	FOnAttackMontageEndedDelegate OnAttackMontageEndedDelegate;

	void SpawnDamagePopup(float DamageAmount);

};
