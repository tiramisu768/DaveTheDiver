// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorComponent/StateComponent/RoboComponent.h"
#include "Controller/MyCharacterController/MyCharacterController.h"
#include "UI/MainUI.h"
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

	if (CurrentHP == 0 && OldHP > 0)
	{
		//게임 종료
		ZeroOxygen();
	}

	else if (CurrentHP == 50 && OldHP > 50)
	{
		//산소 경고
		WarningOxygen();
	}
}

void URoboComponent::InitOxygen()
{
	SetHP(MaxHP);
}

void URoboComponent::ConsumeOxygen(float DeltaTime)
{
	SetHP(CurrentHP - (OxygenConsumptionRate * DeltaTime));
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
	//화면에 붉은 경고 위젯 띄우기
	if (AMyCharacterController* controller = Cast<AMyCharacterController>(GetWorld()->GetFirstPlayerController()))
	{
		if (UMainUI* MainUI = controller->GetMainUI())
		{
			MainUI->ShowHPWarningWidget();
		}
	}
}

void URoboComponent::ZeroOxygen()
{
	//OnOxygenDepleted.Broadcast();

	if (AMyCharacterController* controller = Cast<AMyCharacterController>(GetWorld()->GetFirstPlayerController()))
	{
		if (UMainUI* MainUI = controller->GetMainUI())
		{
			MainUI->ShowGameResultUI(false);
		}
	}
}

void URoboComponent::TakeDamage(float DamageAmount, const FHitResult& HitResult)
{
	Super::TakeDamage(DamageAmount,HitResult);
}

void URoboComponent::Heal(float HealAmount)
{
	Super::Heal(HealAmount);
}

// Called every frame
void URoboComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CurrentDepth > 0.0f) 
	{
		ConsumeOxygen(DeltaTime); 
	}
}

void URoboComponent::UpdateCurrentDepth(float NewDepth)
{
	if (!FMath::IsNearlyEqual(CurrentDepth, NewDepth, 0.01f))
	{
		CurrentDepth = NewDepth;
		OnDepthChanged.ExecuteIfBound(CurrentDepth);
	}
}
