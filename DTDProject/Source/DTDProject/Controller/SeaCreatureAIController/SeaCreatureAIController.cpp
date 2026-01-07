// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/SeaCreatureAIController/SeaCreatureAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "SeaCreature/SeaCreature.h"

const FName ASeaCreatureAIController::TargetActorKey = TEXT("TargetActor");
const FName ASeaCreatureAIController::HomeLocationKey = TEXT("HomeLocation");
const FName ASeaCreatureAIController::IsFarFromHomeKey = TEXT("IsFarFromHome");
const FName ASeaCreatureAIController::IsThreatImminentKey = TEXT("IsThreatImminent");
const FName ASeaCreatureAIController::IsChargingKey = TEXT("IsCharging");
const FName ASeaCreatureAIController::ChaseTargetLocationKey = TEXT("ChaseTargetLocation");
const FName ASeaCreatureAIController::IsReadyToAttackKey = TEXT("IsReadyToAttack");
const FName ASeaCreatureAIController::LastAttackEndTimeKey = TEXT("LastAttackEndTime");
const FName ASeaCreatureAIController::FaceStartTimeKey = TEXT("FaceStartTime");

ASeaCreatureAIController::ASeaCreatureAIController()
{
	PerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

	TeamId = FGenericTeamId(1);
}

void ASeaCreatureAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void ASeaCreatureAIController::PlayBehaviorTree(APawn* InPawn)
{
	ASeaCreature* SeaCreature = Cast<ASeaCreature>(InPawn);
	if (SeaCreature == nullptr) return;

	const FSeaCreatureData* FishData = SeaCreature->GetData();
	if (FishData == nullptr) return;

	UBehaviorTree* BTToRun = nullptr;
	if (FishData->Disposition == ESeaDisposition::Territorial)
	{
		BTToRun = TerritorialBT;
	}
	else if (FishData->Disposition == ESeaDisposition::Aggressive)
	{
		BTToRun = AggressiveBT;
	}
	else
	{
		BTToRun = PassiveBT;
		if (BTToRun)
		{
			UE_LOG(LogTemp, Log, TEXT("%s is Passive. Found PassiveBT: %s"), *InPawn->GetName(), *BTToRun->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("%s is Passive, but PassiveBT variable is NOT SET in the AI Controller Blueprint!"), *InPawn->GetName());
		}
	}

	if (BTToRun == nullptr) return;

	if (SightConfig)
	{
		SightConfig->SightRadius = FishData->SightRadius; //기본 감지 반경
		SightConfig->LoseSightRadius = FishData->SightRadius * 1.2f; //잃는 반경 (버퍼)
		SightConfig->PeripheralVisionAngleDegrees = 180.f; //시야각 (전방)
		SightConfig->SetMaxAge(5.0f);

		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

		PerceptionComp->ConfigureSense(*SightConfig);
		PerceptionComp->SetDominantSense(SightConfig->GetSenseImplementation());
	}

	UBlackboardComponent* BlackboardComp = GetBlackboardComponent();
	if (UseBlackboard(BlackboardAsset, BlackboardComp))
	{
		if (RunBehaviorTree(BTToRun))
		{
			BlackboardComp->SetValueAsVector(HomeLocationKey, SeaCreature->GetActorLocation());
			BlackboardComp->SetValueAsBool(IsFarFromHomeKey, false);
		}
	}
}

FGenericTeamId ASeaCreatureAIController::GetGenericTeamId() const
{
	return TeamId;
}
