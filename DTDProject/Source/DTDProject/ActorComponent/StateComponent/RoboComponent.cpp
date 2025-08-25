// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorComponent/StateComponent/RoboComponent.h"
#include "Controller/MyCharacterController/MyCharacterController.h"
#include "HUD/MyHUD.h"

#include "Engine/World.h"

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
	HP = MaxHP;
	if (HP < 0.0f) HP = 0.0f;
	OnHPChanged.ExecuteIfBound(GetHPPercent()); //델리게이트 실행

	//근접무기 초기화 - 로봇에서 관리

	//원거리무기 초기화 - 로봇에서 관리

	//포획가방 초기화 - 포획가방 컴포넌트 따로 제작해서 관리
}

void URoboComponent::DecreaseO2()
{
	if (bIsDiving)
	{
		GEngine->AddOnScreenDebugMessage(-3, 2.0f, FColor::Red, FString::Printf(TEXT("HP: %f"), HP));
		HP = FMath::Max(0.f, HP - 1.f);

		OnHPChanged.ExecuteIfBound(GetHPPercent());
	}
}

// Called every frame
void URoboComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}