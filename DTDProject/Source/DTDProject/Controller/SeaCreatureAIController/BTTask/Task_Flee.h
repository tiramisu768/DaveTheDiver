// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Task_Flee.generated.h"

/**
 * 
 */
UCLASS()
class DTDPROJECT_API UTask_Flee : public UBTTaskNode
{
	GENERATED_BODY()
	UTask_Flee() { bNotifyTick = true; }
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override
	{ return EBTNodeResult::InProgress; }
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
