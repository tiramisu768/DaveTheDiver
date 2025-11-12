// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SeaCreatureAIController.generated.h"

class UAIPerceptionComponent;
class UAISenseConfig_Sight;

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

	FGenericTeamId TeamId;

public:
	ASeaCreatureAIController();

	static const FName TargetActorKey;
	static const FName HomeLocationKey;
	static const FName MoveDirectionKey;
	static const FName IsThreatNearbyKey;

	virtual FGenericTeamId GetGenericTeamId() const override;
	void PlayBehaviorTree(APawn* InPawn);

protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "AI", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UAIPerceptionComponent> PerceptionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr <UAISenseConfig_Sight> SightConfig;

	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float DeltaSeconds) override;
};
