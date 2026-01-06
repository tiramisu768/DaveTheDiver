// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Task_ChargeStraight.generated.h"

/**
 * 
 */
UCLASS()
class DTDPROJECT_API UTask_ChargeStraight : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UTask_ChargeStraight();

	UPROPERTY(EditAnywhere, Category = "Settings")
	float MaxChargeDistance = 2000.0f;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
