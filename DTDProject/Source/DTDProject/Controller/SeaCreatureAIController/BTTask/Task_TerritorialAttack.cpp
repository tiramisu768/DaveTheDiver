// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/SeaCreatureAIController/BTTask/Task_TerritorialAttack.h"
#include "Controller/SeaCreatureAIController/SeaCreatureAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "SeaCreature/SeaCreature.h"
#include "Controller/SeaCreatureAIController/SeaCreatureSteeringComponent.h"
#include "MyRobo/MyRobo.h"

UTask_TerritorialAttack::UTask_TerritorialAttack()
{
	// 이 태스크는 한 번만 실행되고 몽타주가 끝날 때까지 기다리므로, Tick이 필요 없음
	bNotifyTick = false;
	bNotifyTaskFinished = true;
	NodeName = TEXT("Attack");
}

EBTNodeResult::Type UTask_TerritorialAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp) return EBTNodeResult::Failed;

	ASeaCreature* SeaCreature = Cast<ASeaCreature>(OwnerComp.GetAIOwner()->GetPawn());
	if (SeaCreature == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	const FSeaCreatureData* Data = SeaCreature->GetData();
	if (Data == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UTask_Attack::ExecuteTask - SeaCreature Data is null"));
		return EBTNodeResult::Failed;
	}
	if (Data->Disposition == ESeaDisposition::Passive)
	{
		UE_LOG(LogTemp, Verbose, TEXT("UTask_Attack aborted: Creature is Passive (no attack)."));
		return EBTNodeResult::Failed;
	}

	AMyRobo* Target = Cast<AMyRobo>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ASeaCreatureAIController::TargetActorKey));
	if (Target == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	SeaCreature->OnAttackMontageEndedDelegate.BindLambda([this, &OwnerComp]()
		{
			UBlackboardComponent* BBPtr = OwnerComp.GetBlackboardComponent();
			if (BBPtr && OwnerComp.GetWorld())
			{
				BBPtr->SetValueAsFloat(ASeaCreatureAIController::LastAttackEndTimeKey, OwnerComp.GetWorld()->GetTimeSeconds());
			}

			this->FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		});

	SeaCreature->Attack(Target);

	return EBTNodeResult::InProgress;
}

void UTask_TerritorialAttack::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type Result)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, Result);

	// 태스크가 어떤 이유로든(성공, 실패, 중단) 끝나면 IsCharging 상태를 해제
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsBool(ASeaCreatureAIController::IsChargingKey, false);
		BlackboardComp->SetValueAsBool(ASeaCreatureAIController::IsReadyToAttackKey,false);
		BlackboardComp->ClearValue(ASeaCreatureAIController::ChaseTargetLocationKey);
		BlackboardComp->ClearValue(ASeaCreatureAIController::FaceStartTimeKey);
	}

	// 델리게이트 바인딩 해제
	ASeaCreature* SeaCreature = Cast<ASeaCreature>(OwnerComp.GetAIOwner()->GetPawn());
	if (SeaCreature && SeaCreature->OnAttackMontageEndedDelegate.IsBound())
	{
		SeaCreature->OnAttackMontageEndedDelegate.Unbind();
	}

}

