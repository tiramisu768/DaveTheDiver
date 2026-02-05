// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/StateComponent/StateComponent.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values for this component's properties
UStateComponent::UStateComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> HitEffectFinder(TEXT("/Game/BluePrint/Effect/P_Smoke_A.P_Smoke_A"));
	if (HitEffectFinder.Succeeded())
	{
		HitEffect = HitEffectFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> SwimBubbleEffectFinder(TEXT("/Game/BluePrint/Effect/P_Bubbles_A.P_Bubbles_A"));
	if (SwimBubbleEffectFinder.Succeeded())
	{
		SwimBubbleEffect = SwimBubbleEffectFinder.Object;
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
			HitEffectComponent = UGameplayStatics::SpawnEmitterAttached(
				HitEffect,
				OwnerMesh,
				NAME_None,
				FVector::ZeroVector,
				FRotator::ZeroRotator,
				EAttachLocation::SnapToTarget,
				false
			);
			if (HitEffectComponent)
			{
				HitEffectComponent->SetVisibility(false);
			}
		}
	}

	if (SwimBubbleEffect && !SwimBubbleEffectComponent)
	{
		USkeletalMeshComponent* OwnerMesh = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
		if(OwnerMesh)
		{
			SwimBubbleEffectComponent = UGameplayStatics::SpawnEmitterAttached(
				SwimBubbleEffect,
				OwnerMesh,
				TEXT("BubbleSocket"),
				FVector::ZeroVector,
				FRotator::ZeroRotator,
				EAttachLocation::SnapToTarget,
				false
			);

			if (SwimBubbleEffectComponent)
			{
				SwimBubbleEffectComponent->SetVisibility(false);
			}
		}
	}

}

void UStateComponent::HideHitEffect()
{
	if (HitEffectComponent && CurrentHP > 10.0f)
	{
		HitEffectComponent->SetVisibility(false);
	}
}

void UStateComponent::SetHP(float NewHP)
{
	float OldHP = CurrentHP;
	CurrentHP = FMath::Clamp(NewHP, 0.f, MaxHP); //체력이 음수가 되거나 최대치 초과를 차단
	if(FMath::IsNearlyEqual(OldHP,CurrentHP)==false)
	{
		OnHPChanged.ExecuteIfBound(GetHPPercent());
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

	SetHP(CurrentHP - DamageAmount);
	OnTakeDamage.ExecuteIfBound(DamageAmount);

	if (HitEffectComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Blood Start"));
		HitEffectComponent->SetVisibility(true);

		if (CurrentHP > 10.0f)
		{
			GetWorld()->GetTimerManager().SetTimer(HitEffectTimerHandle, this, &UStateComponent::HideHitEffect, 5.0f,false);
		}
		else
		{
			GetWorld()->GetTimerManager().ClearTimer(HitEffectTimerHandle);
			SetComponentTickEnabled(true);
		}
	}
}

void UStateComponent::ShowSwimBubbleEffect(bool bShow)
{
	if (SwimBubbleEffectComponent)
	{
		SwimBubbleEffectComponent->SetVisibility(bShow);
	}
}

void UStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CurrentHP <= 10.0f && !IsDead())
	{
		if (HitEffectComponent && !HitEffectComponent->IsVisible())
		{
			HitEffectComponent->SetVisibility(true);
		}
	}
	else
	{
		SetComponentTickEnabled(false);
		if (!GetWorld()->GetTimerManager().IsTimerActive(HitEffectTimerHandle))
		{
			HideHitEffect();
		}
	}
}
