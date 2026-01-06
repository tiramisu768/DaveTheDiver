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

public:
	ASeaCreatureAIController();

	static const FName TargetActorKey;
	static const FName HomeLocationKey;
	static const FName IsFarFromHomeKey;
	static const FName IsThreatImminentKey; // 도망쳐야 할 만큼 가까운가?
	static const FName IsChargingKey;
	static const FName ChaseTargetLocationKey; //
	static const FName IsReadyToAttackKey;
	static const FName LastAttackEndTimeKey;
	static const FName FaceStartTimeKey;

	virtual FGenericTeamId GetGenericTeamId() const override;

	void PlayBehaviorTree(APawn* InPawn);

protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<class UBehaviorTree> PassiveBT;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<class UBehaviorTree> AggressiveBT;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<class UBehaviorTree> MonsterBT;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<class UBlackboardData> BlackboardAsset;

	FGenericTeamId TeamId;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "AI", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UAIPerceptionComponent> PerceptionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr <UAISenseConfig_Sight> SightConfig;
};
