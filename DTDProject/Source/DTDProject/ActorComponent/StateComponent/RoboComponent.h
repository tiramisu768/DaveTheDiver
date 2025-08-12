// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponent/StateComponent/StateComponent.h"
#include "Components/WidgetComponent.h"
#include "RoboComponent.generated.h"

/**
 * 
 */
UCLASS()
class DTDPROJECT_API URoboComponent : public UStateComponent
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, Category = "State")
	float CurrentO2{ 100.0f };
	UPROPERTY(EditAnywhere, Category = "State")
	float MaxO2{ 100.0f };
	UPROPERTY(EditAnywhere, Category = "State")
	float O2DecreaseInterval = 3.0f;
	float O2Timer = 0.0f;
	bool bIsDiving = true; // 수면체크하게 되면 false로 바꾸기
	UPROPERTY(EditAnywhere, Category = "State")
	UWidgetComponent* RoboHPBarWidget;

public:
	void StartDiving();
	void StopDriving();
	void UpdateO2ProgressBar();
	void InitO2()
	{
		CurrentO2 = MaxO2;
		if (CurrentO2 < 0.0f) CurrentO2 = 0.0f;
		//OnTakeDamage.ExecuteIfBound(GetHPPercent());
	}
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
