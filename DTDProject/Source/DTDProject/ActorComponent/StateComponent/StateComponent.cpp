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

void UStateComponent::TakeDamage(float DamageAmount)
{
	if (DamageAmount <= 0.0f || HP <= 0.0f)
		return;
	HP -= DamageAmount;
	if (HP < 0.0f)
		HP = 0.0f;
	if (OnTakeDamage.IsBound())
	{
		OnTakeDamage.Execute(GetHPPercent());
	}
}


void UStateComponent::StartDiving()
{
	bIsDiving = true;
}

void UStateComponent::StopDriving()
{
	bIsDiving = false;
}

// Called every frame
void UStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsDiving)
	{
		O2Timer += DeltaTime;
		if (O2Timer >= O2DecreaseInterval)
		{
			HP = FMath::Max(0.f, HP - 1.f);
			O2Timer = 0;
		}
	}
}
