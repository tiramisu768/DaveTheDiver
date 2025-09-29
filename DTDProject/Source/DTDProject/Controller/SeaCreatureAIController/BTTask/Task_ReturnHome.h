// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Task_ReturnHome.generated.h"

/**
 * 
 */
UCLASS()
class DTDPROJECT_API UTask_ReturnHome : public UBTTaskNode
{
	GENERATED_BODY()
	UTask_ReturnHome() { bNotifyTick = true; }

	UPROPERTY(EditAnywhere, Category = "Blackboard");
	FBlackboardKeySelector HomeLocationKey;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override
	{
	  return	EBTNodeResult::InProgress;
	}
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
