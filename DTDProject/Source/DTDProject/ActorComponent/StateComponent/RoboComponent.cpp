// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorComponent/StateComponent/RoboComponent.h"

void URoboComponent::StartDiving()
{
	bIsDiving = true;
}

void URoboComponent::StopDriving()
{
	bIsDiving = false;
}

void URoboComponent::UpdateO2ProgressBar()
{
	//if (RoboHPBarWidget) // UWidgetComponent*
	//{
	//	URoboHPBarUI* HPBarUI = Cast<URoboHPBarUI>(RoboHPBarWidget->GetUserWidgetObject());
	//	if (HPBarUI)
	//	{
	//		float Percent = CurrentO2 / MaxO2;
	//		HPBarUI->SetOxygenPercent(Percent);
	//	}
	//}
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

			UpdateO2ProgressBar();
		}
	}
}