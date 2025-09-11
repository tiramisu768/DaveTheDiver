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
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<class UBehaviorTree> PassiveBT;
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<class UBehaviorTree> AggressiveBT;
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<class UBlackboardData> BlackboardAsset;
	UPROPERTY(VisibleAnywhere, Category = "AI")
	class UBlackboardComponent* BlackboardComp;

public:
	ASeaCreatureAIController();
	void PlayBehaviorTree(APawn* InPawn);

protected:
	UPROPERTY(VisibleAnywhere, Category = "AI")
	class UAIPerceptionComponent* PerceptionComp;
	UPROPERTY()
	class UAISenseConfig_Sight* SightConfig;

	void OnPossess(APawn* InPawn) override;
};
