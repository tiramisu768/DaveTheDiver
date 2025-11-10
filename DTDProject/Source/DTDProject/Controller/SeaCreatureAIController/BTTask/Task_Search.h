// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Task_Search.generated.h"

/**
 * 
 */
UCLASS()
class DTDPROJECT_API UTask_Search : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UTask_Search();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category="Search")
	float SearchTime = 3.0f;
};
