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
			DecreaseOxygen();
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

void URoboComponent::SetHP(float NewHP)
{
	float OldHP = CurrentHP;
	Super::SetHP(NewHP);

	if (CurrentHP == 90 && OldHP > 90)
	{
		//게임 종료
		ZeroOxygen();
	}

	else if (CurrentHP == 95 && OldHP > 95)
	{
		//산소 경고
		WarningOxygen();
	}
}

void URoboComponent::InitOxygen()
{
	SetHP(MaxHP);

	//근접무기 초기화 - 로봇에서 관리

	//원거리무기 초기화 - 로봇에서 관리

	//포획가방 초기화 - 포획가방 컴포넌트 따로 제작해서 관리
}

void URoboComponent::DecreaseOxygen()
{
	if (bIsDiving)
	{
		SetHP(CurrentHP - 1.f);
	}
}

void URoboComponent::WarningOxygen()
{
	//화면에 붉은 UI 띄우기 :hud를 호출?
	if (APlayerController* controller = GetWorld()->GetFirstPlayerController())
	{
		if (AMyHUD* MyHUD = Cast<AMyHUD>(controller->GetHUD()))
		{
			MyHUD->ShowOxygenWarningUI();
		}
	}
}

void URoboComponent::ZeroOxygen()
{
	// 게임 플레이 일시 중지 :컨트롤러에? 로보에? 어디에 함수 작성하는가
	// 산소타이머 일시 중지 :방법 찾기
	// 획득한 물고기 UI 띄우기 :hud를 호출?
	if (APlayerController* controller = GetWorld()->GetFirstPlayerController())
	{
		if (AMyHUD* MyHUD = Cast<AMyHUD>(controller->GetHUD()))
		{
			MyHUD->ShowGameEndUI();
		}
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