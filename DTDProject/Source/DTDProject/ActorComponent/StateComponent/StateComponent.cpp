// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/StateComponent/StateComponent.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values for this component's properties
UStateComponent::UStateComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UStateComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}

void UStateComponent::SetHP(float NewHP)
{
	CurrentHP = FMath::Clamp(NewHP, 0, MaxHP); //체력이 음수가 되거나 최대치 초과를 차단
	OnHPChanged.ExecuteIfBound(GetHPPercent());
}

void UStateComponent::SetMaxHP(float NewMaxHP)
{
	MaxHP = FMath::Max(0.0f, NewMaxHP); //최대 체력이 음수가 되는 상황을 방지
	SetHP(MaxHP);
}

void UStateComponent::TakeDamage(float DamageAmount, const FHitResult& HitResult)
{
	if (IsDead() || DamageAmount <= 0.f)
	{
		return;
	}

	SetHP(CurrentHP - DamageAmount);
	OnTakeDamage.ExecuteIfBound(DamageAmount);

	if (HitEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),
			HitEffect,
			HitResult.ImpactPoint,
			HitResult.ImpactNormal.Rotation()
		);
	}

	if (IsDead())
	{

	}
}

//회복
void UStateComponent::Heal(float HealAmount)
{
	if (HealAmount <= 0.0f || IsDead())
		return;

	SetHP(CurrentHP + HealAmount);
}

// Called every frame
void UStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
