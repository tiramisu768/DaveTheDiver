// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SeaCreatureAIController.generated.h"

/**
 * 
 */
UCLASS()
class DTDPROJECT_API ASeaCreatureAIController : public AAIController
{
	GENERATED_BODY()
private:
	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BehaviorTreeAsset;
	UPROPERTY()
	TObjectPtr<class UBlackboardData> BlackboardAsset;

public:
	ASeaCreatureAIController();
	void PlayBehaviorTree();

protected:
	void OnPossess(APawn* InPawn) override;
};
