// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/SeaCreatureAIController/BTTask/Task_Search.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UTask_Search::UTask_Search()
{
	bNotifyTick = true;
	NodeName = TEXT("Search For Target");
}

EBTNodeResult::Type UTask_Search::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(TEXT("SearchStartTime"), GetWorld()->GetTimeSeconds());
	return EBTNodeResult::InProgress;
}

void UTask_Search::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	const float StartTime = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(TEXT("SearchStartTime"));
	const float ElapsedTime = GetWorld()->GetTimeSeconds() - StartTime;

	// 만약 탐색 중에 플레이어를 다시 발견하면, 즉시 "실패"를 반환하여 상위 노드(Seek)로 돌아가게 합니다.
	if (OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("TargetActor")) != nullptr)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	// 설정된 탐색 시간이 지나면, "성공"을 반환하여 Wander 상태로 돌아갑니다.
	if (ElapsedTime > SearchTime)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	// 시간이 다 되기 전까지는 제자리에서 계속 회전합니다.
	APawn* OwnerPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (OwnerPawn)
	{
		OwnerPawn->AddActorLocalRotation(FRotator(0.f, 180.f * DeltaSeconds, 0.f));
	}
}
