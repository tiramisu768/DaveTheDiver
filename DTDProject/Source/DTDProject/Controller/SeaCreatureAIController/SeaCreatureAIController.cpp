// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/SeaCreatureAIController/SeaCreatureAIController.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTree.h"

ASeaCreatureAIController::ASeaCreatureAIController()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BehaviorTreeFinder(TEXT("/Script/AIModule.BehaviorTree'/Game/BluePrint/SeaCreature/AI/BT_SeaCreature.BT_SeaCreature'"));
	if (BehaviorTreeFinder.Succeeded())
		BehaviorTreeAsset = BehaviorTreeFinder.Object;

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BlackboardFinder(TEXT("/Script/AIModule.BlackboardData'/Game/BluePrint/SeaCreature/AI/BB_SeaCreature.BB_SeaCreature'"));
	if (BlackboardFinder.Succeeded())
		BlackboardAsset = BlackboardFinder.Object;

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

void ASeaCreatureAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	PlayBehaviorTree();
}
