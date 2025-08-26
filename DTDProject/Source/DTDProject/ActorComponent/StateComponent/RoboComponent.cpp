// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorComponent/StateComponent/RoboComponent.h"
#include "Controller/MyCharacterController/MyCharacterController.h"
#include "HUD/MyHUD.h"

#include "Engine/World.h"

URoboComponent::URoboComponent()
{
}

void URoboComponent::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(O2TimerHandle, [this]()
		{
			DecreaseO2();
		}
		,3.0f,true
	);
}

void URoboComponent::StartDiving()
{
	bIsDiving = true;
}

void URoboComponent::StopDriving()
{
	bIsDiving = false;
}

void URoboComponent::InitRoboUIStatement()
{
	//산소량 초기화
	CurrentHP = MaxHP;
	if (CurrentHP < 0.0f) CurrentHP = 0.0f;
	OnHPChanged.ExecuteIfBound(GetHPPercent()); //델리게이트 실행

	//근접무기 초기화 - 로봇에서 관리

	//원거리무기 초기화 - 로봇에서 관리

	//포획가방 초기화 - 포획가방 컴포넌트 따로 제작해서 관리
}

void URoboComponent::DecreaseO2()
{
	if (bIsDiving)
	{
		GEngine->AddOnScreenDebugMessage(-3, 2.0f, FColor::Red, FString::Printf(TEXT("HP: %f"), CurrentHP));
		CurrentHP = FMath::Max(0.f, CurrentHP - 1.f);

		OnHPChanged.ExecuteIfBound(GetHPPercent());
	}
}

void URoboComponent::TakeDamage(float DamageAmount)
{
	Super::TakeDamage(DamageAmount);
}

void URoboComponent::Heal(float HealAmount)
{
	Super::Heal(HealAmount);
}

// Called every frame
void URoboComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}