// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/SeaCreatureAIController/SeaCreatureAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "SeaCreature/SeaCreature.h"
#include "GameFramework/FloatingPawnMovement.h"

const FName ASeaCreatureAIController::TargetActorKey = TEXT("TargetActor");
const FName ASeaCreatureAIController::HomeLocationKey = TEXT("HomeLocation");
const FName ASeaCreatureAIController::MoveDirectionKey = TEXT("MoveDirection");
const FName ASeaCreatureAIController::IsThreatNearbyKey = TEXT("IsThreatNearby");

ASeaCreatureAIController::ASeaCreatureAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BlackboardFinder(TEXT("/Script/AIModule.BlackboardData'/Game/BluePrint/SeaCreature/AI/BB_SeaCreature.BB_SeaCreature'"));
	if (BlackboardFinder.Succeeded())
		BlackboardAsset = BlackboardFinder.Object;

	//Perception
	PerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	if(SightConfig)
	{
		SightConfig->SightRadius = 5000.f; //기본 감지 반경
		SightConfig->LoseSightRadius = 960.f; //잃는 반경 (버퍼)
		SightConfig->PeripheralVisionAngleDegrees = 180.f; //시야각 (전방)
		SightConfig->SetMaxAge(5.0f);

		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

		PerceptionComp->ConfigureSense(*SightConfig);
		PerceptionComp->SetDominantSense(SightConfig->GetSenseImplementation());
	}

	TeamId = FGenericTeamId(1);

}

FGenericTeamId ASeaCreatureAIController::GetGenericTeamId() const
{
	return TeamId;
}

void ASeaCreatureAIController::PlayBehaviorTree(APawn* InPawn)
{
	ASeaCreature* SeaCreature = Cast<ASeaCreature>(InPawn);
	if (!SeaCreature) return;

	// OverrideBT가 유효한지 확인합니다.
	if (SeaCreature->OverrideBT == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ASeaCreatureAIController::PlayBehaviorTree - SeaCreature '%s' has no OverrideBT set!"), *SeaCreature->GetName());
		return;
	}

	UBlackboardComponent* BlackboardComp = Blackboard.Get();
	if (!UseBlackboard(BlackboardAsset, BlackboardComp)) return;

	if (!RunBehaviorTree(SeaCreature->OverrideBT))
	{
		UE_LOG(LogTemp, Error, TEXT("ASeaCreatureAIController::PlayBehaviorTree - Failed to run Behavior Tree for %s!"), *SeaCreature->GetName());
		return;
	}

	BlackboardComp->SetValueAsVector(TEXT("HomeLocation"), SeaCreature->GetActorLocation());

	const FSeaCreatureData* FishData = SeaCreature->GetData();
	if (FishData && SightConfig)
	{
		SightConfig->SightRadius = FishData->SightRadius;
		PerceptionComp->ConfigureSense(*SightConfig);
	}
}

void ASeaCreatureAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	PlayBehaviorTree(InPawn);
}

void ASeaCreatureAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	/*ASeaCreature* SeaCreature = Cast<ASeaCreature>(GetPawn());
	UBlackboardComponent* BlackboardComp = GetBlackboardComponent();

	if (SeaCreature && BlackboardComp)
	{
		FVector Dir = BlackboardComp->GetValueAsVector(MoveDirectionKey);
		if (!Dir.IsNearlyZero())
		{
			SeaCreature->AddMovementInput(Dir);

			FRotator TargetRot = Dir.Rotation();
			SeaCreature->SetActorRotation(FMath::RInterpTo(SeaCreature->GetActorRotation(), TargetRot, DeltaSeconds, 2.0f));
		}
	}*/
}
