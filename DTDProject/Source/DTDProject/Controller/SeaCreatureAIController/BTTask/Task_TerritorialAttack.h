// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Task_TerritorialAttack.generated.h"

/**
 * 
 */
UCLASS()
class DTDPROJECT_API UTask_TerritorialAttack : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UTask_TerritorialAttack();
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type Result) override;
};
