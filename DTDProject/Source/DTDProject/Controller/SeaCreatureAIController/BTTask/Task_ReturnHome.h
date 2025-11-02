// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Task_ReturnHome.generated.h"

/**
 * 
 */
UCLASS()
class DTDPROJECT_API UTask_ReturnHome : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UTask_ReturnHome();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
