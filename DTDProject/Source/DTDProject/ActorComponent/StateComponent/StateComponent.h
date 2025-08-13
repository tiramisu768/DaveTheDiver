// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StateComponent.generated.h"

DECLARE_DELEGATE_OneParam(FOnTakeDamage, float);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DTDPROJECT_API UStateComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "State")
	float HP{ 100.0f };
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
	FOnTakeDamage OnTakeDamage;

public:
	void TakeDamage(float DamageAmount);
	float GetDamage() const { return Damage; }
	float GetHP() const { return HP; }
	void SetDamage(float NewDamage) { Damage = NewDamage; }
	void SetHP(float NewHP)
	{
		HP = MaxHP = NewHP;
		if (HP < 0.0f) HP = 0.0f;
		OnTakeDamage.ExecuteIfBound(GetHPPercent());
	}
	float GetHPPercent() const { return HP / MaxHP; }
	bool isDead() const { return HP <= 0.0f; }
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
