// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/StateComponent/StateComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

// Sets default values for this component's properties
UStateComponent::UStateComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> HitEffectFinder(TEXT("/Game/StackOBot/FX/JetpackThruster/FX_JetpackThruster.FX_JetpackThruster"));
	if (HitEffectFinder.Succeeded())
	{
		HitEffect = HitEffectFinder.Object;
	}
}


// Called when the game starts
void UStateComponent::BeginPlay()
{
	Super::BeginPlay();

	if (HitEffect && !HitEffectComponent)
	{
		USkeletalMeshComponent* OwnerMesh = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
		if (OwnerMesh)
		{
			HitEffectComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
				HitEffect,
				OwnerMesh,
				NAME_None,
				FVector::ZeroVector,
				FRotator::ZeroRotator,
				EAttachLocation::SnapToTarget,
				false
			);
		}
	}

}

void UStateComponent::UpdateHitEffect()
{
	if (!HitEffect) return;

	if (!HitEffectComponent)
	{
		USkeletalMeshComponent* OwnerMesh = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
		if (OwnerMesh)
		{
			HitEffectComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
				HitEffect,
				OwnerMesh,
				NAME_None,
				FVector::ZeroVector,
				FRotator::ZeroRotator,
				EAttachLocation::SnapToTarget,
				false
			);
		}
	}

	if (!HitEffectComponent) return;

	if (IsDead())
	{
		HitEffectComponent->Deactivate();
		GetWorld()->GetTimerManager().ClearTimer(HitEffectTimerHandle);
	}
	else if (CurrentHP <= 10.0f)
	{
		HitEffectComponent->Activate(true);
		GetWorld()->GetTimerManager().ClearTimer(HitEffectTimerHandle);
	}
	else
	{
		if (!GetWorld()->GetTimerManager().IsTimerActive(HitEffectTimerHandle))
		{
			HideHitEffect();
		}
	}
}

void UStateComponent::HideHitEffect()
{
	if (HitEffectComponent && CurrentHP > 10.0f)
	{
		HitEffectComponent->Deactivate();
	}
}

void UStateComponent::SetHP(float NewHP)
{
	float OldHP = CurrentHP;
	CurrentHP = FMath::Clamp(NewHP, 0, MaxHP); //체력이 음수가 되거나 최대치 초과를 차단
	if(FMath::IsNearlyEqual(OldHP,CurrentHP)==false)
	{
		OnHPChanged.ExecuteIfBound(GetHPPercent());
		UpdateHitEffect();
	}
}

void UStateComponent::SetMaxHP(float NewMaxHP)
{
	MaxHP = FMath::Max(0.0f, NewMaxHP); //최대 체력이 음수가 되는 상황을 방지
	SetHP(CurrentHP);
}

void UStateComponent::TakeDamage(float DamageAmount, const FHitResult& HitResult)
{
	if (IsDead() || DamageAmount <= 0.f)
	{
		return;
	}

	if (HitEffectComponent)
	{
		HitEffectComponent->Activate(true);
	}

	if (CurrentHP > 10.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(HitEffectTimerHandle, this, &UStateComponent::HideHitEffect, 5.0f);
	}

	SetHP(CurrentHP - DamageAmount);
	OnTakeDamage.ExecuteIfBound(DamageAmount);


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
