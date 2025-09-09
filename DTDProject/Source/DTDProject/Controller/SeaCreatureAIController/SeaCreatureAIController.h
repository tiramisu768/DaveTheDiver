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
	TObjectPtr<class UBehaviorTree> BehaviorTreeAsset;
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<class UBlackboardData> BlackboardAsset;


public:
	ASeaCreatureAIController();
	void PlayBehaviorTree();

protected:
	UPROPERTY(VisibleAnywhere, Category = "AI")
	class UAIPerceptionComponent* PerceptionComp;
	UPROPERTY()
	class UAISenseConfig_Sight* SightConfig;
	UPROPERTY() 
	AActor* Robo = nullptr;
	UFUNCTION()
	void OnPerceptionUpdated(AActor* Robo, FAIStimulus Stimulus);
	void OnPossess(APawn* InPawn) override;
};
