// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponent/StateComponent/StateComponent.h"
#include "Components/WidgetComponent.h"
#include "TimerManager.h"
#include "RoboComponent.generated.h"

/**
 * 
 */
//UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
DECLARE_DELEGATE_OneParam(FOnHPChanged, float);
DECLARE_DELEGATE_OneParam(FOnDepthChanged, float);
DECLARE_DELEGATE_TwoParams(FOnWeightChanged, float, float);
UCLASS()
class DTDPROJECT_API URoboComponent : public UStateComponent
{
	GENERATED_BODY()
private:
	FTimerHandle O2TimerHandle;

	bool bIsDiving = true; // 수면체크하게 되면 false로 바꾸기
	UPROPERTY(EditAnywhere, Category = "State")
	UWidgetComponent* RoboHPBarWidget;

public:
	FOnHPChanged OnHPChanged;
	FOnDepthChanged OnDepthChanged;
	FOnWeightChanged OnWeightChanged;

public:
	void BeginPlay() override;
	void StartDiving();
	void StopDriving();
	void InitRoboUIStatement();
	float GetHPPercent() const { return HP / MaxHP; }
	void DecreaseO2();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
