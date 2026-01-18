// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponent/StateComponent/StateComponent.h"
#include "Components/WidgetComponent.h"
#include "TimerManager.h"
#include "RoboComponent.generated.h"

DECLARE_DELEGATE_OneParam(FOnDepthChanged, float);
DECLARE_MULTICAST_DELEGATE(FOnOxygenDepleted);

UCLASS()
class DTDPROJECT_API URoboComponent : public UStateComponent
{
	GENERATED_BODY()
public:
	FOnDepthChanged OnDepthChanged;
	FOnOxygenDepleted OnOxygenDepleted;

	URoboComponent();
	void StartDiving();
	void StopDriving();
	virtual void SetHP(float NewHP) override;
	void InitOxygen();
	void ConsumeOxygen(float DeltaTime);
	void DecreaseOxygen();
	void WarningOxygen();
	void ZeroOxygen();
	virtual void TakeDamage(float DamageAmount, const FHitResult& HitResult) override;
	virtual void Heal(float HealAmount) override;
	void UpdateCurrentDepth(float NewDepth);

protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void BeginPlay() override;
	void EndPlay(const EEndPlayReason::Type EndPlayReason);

	UPROPERTY(EditAnywhere, Category = "Robo State | Oxygen")
	float OxygenConsumptionRate = 1.0f; // 초당 산소 소모량

private:
	FTimerHandle O2TimerHandle;

	bool bIsDiving = true; // 수면체크하게 되면 false로 바꾸기
	UPROPERTY(EditAnywhere, Category = "State | Robo")
	UWidgetComponent* RoboHPBarWidget;
	float CurrentDepth = 0.0f;




};
