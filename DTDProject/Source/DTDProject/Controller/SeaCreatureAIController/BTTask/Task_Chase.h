// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Task_Chase.generated.h"

/**
 * 
 */
UCLASS()
class DTDPROJECT_API UTask_Chase : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UTask_Chase();

	UPROPERTY(EditAnywhere, Category = "Chase")
	float AcceptanceRadius = 150.0f;

	UPROPERTY(EditAnywhere, Category = "Chase")
	float SearchDuration = 3.0f;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
