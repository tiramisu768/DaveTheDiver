// Fill out your copyright notice in the Description page of Project Settings.
//HP 관리, 데미지 적용, 죽음 이벤트 → 로봇/물고기/보스 등 모든 액터 공용
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StateComponent.generated.h"

class UNiagaraSystem;

DECLARE_DELEGATE_OneParam(FOnHPChanged, float);
DECLARE_DELEGATE_OneParam(FOnTakeDamage, float);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DTDPROJECT_API UStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	FOnHPChanged OnHPChanged;
	FOnTakeDamage OnTakeDamage;

	UStateComponent();

	//void SetDamage(float NewDamage) { Damage = NewDamage; }
	float GetHPPercent() const { return CurrentHP / MaxHP; }
	virtual void SetHP(float NewHP);
	virtual void SetMaxHP(float NewMaxHP);
	virtual void TakeDamage(float DamageAmount, const FHitResult& HitResult);
	virtual void Heal(float HealAmount);
	bool IsDead() const { return CurrentHP <= 0.0f; }
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, Category = "State")
	float CurrentHP{ 100.0f };
	UPROPERTY(EditAnywhere, Category = "State")
	float MaxHP{ 100.0f };

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TObjectPtr<UNiagaraSystem> HitEffect;
};
