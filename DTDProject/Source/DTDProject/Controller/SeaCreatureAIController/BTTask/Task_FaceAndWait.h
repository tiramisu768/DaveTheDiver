// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Task_FaceAndWait.generated.h"

/**
 * 
 */
UCLASS()
class DTDPROJECT_API UTask_FaceAndWait : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UTask_FaceAndWait();

	UPROPERTY(EditAnywhere, Category="Settings")
	float WaitTime = 10.0f;

	UPROPERTY(EditAnywhere, Category ="Settings")
	float RotationSpeed = 5.0f;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
