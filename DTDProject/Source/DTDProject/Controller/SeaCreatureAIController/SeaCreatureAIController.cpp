// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/SeaCreatureAIController/SeaCreatureAIController.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

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

void ASeaCreatureAIController::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	UBlackboardComponent* BlackboardComponent = Blackboard.Get();
	if (!BlackboardComponent) return;
	if (!IsValid(Actor))  return;

	// 자기 자신은 무시
	if (Actor == GetPawn()) return;

	// (선택) 플레이어만 타깃으로 삼고 싶다면:
	// if (!Actor->IsA(AYourPlayerCharacter::StaticClass())) return;

	static const FName Key_Target(TEXT("TargetActor"));
	static const FName Key_LastKnown(TEXT("LastKnownLocation")); // Vector key (선택)

	if (Stimulus.WasSuccessfullySensed())
	{
		// 감지되면 타깃으로 설정
		BlackboardComponent->SetValueAsObject(Key_Target, Actor);
		// 최신 위치 갱신(선택)
		BlackboardComponent->SetValueAsVector(Key_LastKnown, Actor->GetActorLocation());
	}
	else
	{
		// 시야에서 잃었을 때: 현재 저장된 타깃이 이 Actor라면 해제
		UObject* Cur = BlackboardComponent->GetValueAsObject(Key_Target);
		if (Cur == Actor)
		{
			// 마지막으로 감지된 위치만 남겨두고 타깃 해제(선택)
			BlackboardComponent->SetValueAsVector(Key_LastKnown, Stimulus.StimulusLocation);
			BlackboardComponent->ClearValue(Key_Target);
		}
	}
}

void ASeaCreatureAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	PlayBehaviorTree();
}


