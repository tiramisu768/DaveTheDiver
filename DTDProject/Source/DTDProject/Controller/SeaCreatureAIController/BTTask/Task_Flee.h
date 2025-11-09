// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Task_Flee.generated.h"

/**
 * 
 */
UCLASS()
class DTDPROJECT_API UTask_Flee : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UTask_Flee();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere,Category="Flee")
	float MinFleeTime = 3.0f;
};
