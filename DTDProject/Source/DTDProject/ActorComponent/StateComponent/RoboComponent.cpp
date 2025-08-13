// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorComponent/StateComponent/RoboComponent.h"
#include "Controller/MyCharacterController/MyCharacterController.h"
#include "HUD/MyHUD.h"

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
	CurrentO2 = MaxO2;
	if (CurrentO2 < 0.0f) CurrentO2 = 0.0f;
	OnO2Changed.ExecuteIfBound(GetO2Percent());

	//근접무기 초기화

	//원거리무기 초기화

	//포획상자 초기화
}

void URoboComponent::UpdateRoboUIStatement()
{
	if (APawn* PawnOwner = Cast<APawn>(GetOwner()))
	{
		if (AMyCharacterController* MyController = Cast<AMyCharacterController>(PawnOwner->GetController()))
		{
			if (AMyHUD* MyHUD = Cast<AMyHUD>(MyController->GetHUD()))
			{
				float Percent = CurrentO2 / MaxO2;
				MyHUD->SetHPPercent(Percent);
			}
		}
	}

}

// Called every frame
void URoboComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsDiving)
	{
		O2Timer += DeltaTime;
		if (O2Timer >= O2DecreaseInterval)
		{
			GEngine->AddOnScreenDebugMessage(-3, 2.0f, FColor::Red, FString::Printf(TEXT("CurrentO2: %f"),CurrentO2));
			CurrentO2 = FMath::Max(0.f, CurrentO2 - 1.f);
			O2Timer = 0;

			UpdateRoboUIStatement();
		}
	}
}