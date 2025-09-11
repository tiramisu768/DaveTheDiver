// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/SeaCreatureAIController/SeaCreatureAIController.h"
#include "SeaCreature/SeaCreature.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

ASeaCreatureAIController::ASeaCreatureAIController()
{
	//BehviorTree
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> AggressiveBTFinder(TEXT("/Script/AIModule.BehaviorTree'/Game/BluePrint/SeaCreature/AI/BT_SeaCreature.BT_Aggressive'"));
	if (AggressiveBTFinder.Succeeded())
		AggressiveBT = AggressiveBTFinder.Object;
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> PassiveBTFinder(TEXT("/Script/AIModule.BehaviorTree'/Game/BluePrint/SeaCreature/AI/BT_SeaCreature.BT_Passive'"));
	if (PassiveBTFinder.Succeeded())
		PassiveBT = PassiveBTFinder.Object;

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BlackboardFinder(TEXT("/Script/AIModule.BlackboardData'/Game/BluePrint/SeaCreature/AI/BB_SeaCreature.BB_SeaCreature'"));
	if (BlackboardFinder.Succeeded())
		BlackboardAsset = BlackboardFinder.Object;

	//Perception
	PerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

	SightConfig->SightRadius = 2000.f;
	SightConfig->LoseSightRadius = 2400.f;
	SightConfig->PeripheralVisionAngleDegrees = 150.f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	PerceptionComp->ConfigureSense(*SightConfig);
	PerceptionComp->SetDominantSense(SightConfig->GetSenseImplementation());

}

void ASeaCreatureAIController::PlayBehaviorTree(APawn* InPawn)
{
	ASeaCreature* SeaCreature = Cast<ASeaCreature>(InPawn);
	if (!SeaCreature) return;

	if (UseBlackboard(BlackboardAsset, BlackboardComp)) 	//UseBlackboard : 원하는 Blackboard를 사용하기 위해 초기화
	{
		BlackboardComp->SetValueAsVector(TEXT("HomeLocation"), SeaCreature->GetActorLocation());
		UBehaviorTree* BT = SeaCreature->OverrideBT ? SeaCreature->OverrideBT : (SeaCreature->Disposition == ESeaDisposition::Aggressive ? AggressiveBT : PassiveBT);
		if(BT)
		{
			ensure(RunBehaviorTree(BT));   //RunBehaviorTree : 원하는 BehaviorTree를 실행
			                               //ensure : 실행이 실패할 경우 Error Log를 발생시키지만 실행은 지속된다
		}
	}
}

void ASeaCreatureAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	PlayBehaviorTree(InPawn);
}


