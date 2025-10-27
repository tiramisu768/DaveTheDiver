// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/StateComponent/StateComponent.h"

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
	CurrentHP = FMath::Clamp(NewHP, 0, MaxHP);
	OnHPChanged.ExecuteIfBound(GetHPPercent());
}

//피격
void UStateComponent::TakeDamage(float DamageAmount)
{
	if (DamageAmount <= 0.0f || CurrentHP <= 0.0f)
		return;
	CurrentHP -= DamageAmount;
	if (CurrentHP < 0.0f)
		CurrentHP = 0.0f;
	if (OnTakeDamage.IsBound())
	{
		OnTakeDamage.Execute(GetHPPercent());
	}

	//if (CurrentHP <= 0.f)
	//{
	//	Die();
	//}
}

//회복
void UStateComponent::Heal(float HealAmount)
{
	if (HealAmount <= 0.0f || CurrentHP <= 0.0f)
		return;
	CurrentHP += HealAmount;
	if (CurrentHP > MaxHP)
		CurrentHP = MaxHP;
	if (OnTakeDamage.IsBound())
	{
		OnTakeDamage.Execute(GetHPPercent());
	}
}

// Called every frame
void UStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
