// Fill out your copyright notice in the Description page of Project Settings.
//HP 관리, 데미지 적용, 죽음 이벤트 → 로봇/물고기/보스 등 모든 액터 공용
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StateComponent.generated.h"

DECLARE_DELEGATE_OneParam(FOnHPChanged, float);
DECLARE_DELEGATE_OneParam(FOnTakeDamage, float);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DTDPROJECT_API UStateComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "State")
	float CurrentHP{ 100.0f };
	UPROPERTY(EditAnywhere, Category = "State")
	float MaxHP{ 100.0f };
	UPROPERTY(EditAnywhere, Category = "State")
	float Damage{ 10.0f };

public:
	// Sets default values for this component's properties
	UStateComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
public:
	FOnHPChanged OnHPChanged;
	FOnTakeDamage OnTakeDamage;

public:
	float GetDamage() const { return Damage; }
	//void SetDamage(float NewDamage) { Damage = NewDamage; }

	float GetHPPercent() const { return CurrentHP / MaxHP; }
	//float GetHP() const { return HP; }
	/*void SetHP(float NewHP)
	{
		HP = MaxHP = NewHP;
		if (HP < 0.0f) HP = 0.0f;
		OnTakeDamage.ExecuteIfBound(GetHPPercent());
	}*/

	virtual void TakeDamage(float DamageAmount);
	virtual void Heal(float HealAmount);
	bool IsDead() const { return CurrentHP <= 0.0f; }
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
