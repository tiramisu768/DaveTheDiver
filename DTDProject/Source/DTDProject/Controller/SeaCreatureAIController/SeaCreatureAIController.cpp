// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/SeaCreatureAIController/SeaCreatureAIController.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionSystem.h"

ASeaCreatureAIController::ASeaCreatureAIController()
{
	//BehviorTree
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BehaviorTreeFinder(TEXT("/Script/AIModule.BehaviorTree'/Game/BluePrint/SeaCreature/AI/BT_SeaCreature.BT_SeaCreature'"));
	if (BehaviorTreeFinder.Succeeded())
		BehaviorTreeAsset = BehaviorTreeFinder.Object;

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
	//PerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &ASeaCreatureAIController::OnPerceptionUpdated);

}

void ASeaCreatureAIController::PlayBehaviorTree()
{
	UBlackboardComponent* BlackboardComponent = Blackboard.Get(); 	//Blackboar : 부모 class인 AAIController의 멤버변수
	if (UseBlackboard(BlackboardAsset, BlackboardComponent)) 	//UseBlackboard : 원하는 Blackboard를 사용하기 위해 초기화
	{
		ensure(RunBehaviorTree(BehaviorTreeAsset));   //RunBehaviorTree : 원하는 BehaviorTree를 실행
		                                              //ensure : 실행이 실패할 경우 Error Log를 발생시키지만 실행은 지속된다
	}
}

//void ASeaCreatureAIController::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
//{
//	if (Stimulus.WasSuccessfullySensed())
//	{
//		BlackboardComp->SetValueAsObject("TargetActor", Actor);
//	}
//	else
//	{
//		BlackboardComp->ClearValue("TargetActor");
//	}
//}

void ASeaCreatureAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	PlayBehaviorTree();
}


