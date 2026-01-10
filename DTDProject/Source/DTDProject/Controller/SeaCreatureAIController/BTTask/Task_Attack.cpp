#include "Controller/SeaCreatureAIController/BTTask/Task_Attack.h"
#include "Controller/SeaCreatureAIController/SeaCreatureAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "SeaCreature/SeaCreature.h"
#include "Controller/SeaCreatureAIController/SeaCreatureSteeringComponent.h"
#include "MyRobo/MyRobo.h"
#include "Animation/AnimInstance.h"

UTask_Attack::UTask_Attack()
{
	// 이 태스크는 한 번만 실행되고 몽타주가 끝날 때까지 기다리므로, Tick이 필요 없음
	bNotifyTick = false;
	bNotifyTaskFinished = true;
	NodeName = TEXT("Attack");
}

EBTNodeResult::Type UTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ASeaCreature* SeaCreature = Cast<ASeaCreature>(OwnerComp.GetAIOwner()->GetPawn());
	if (SeaCreature == nullptr)	return EBTNodeResult::Failed;

	UAnimInstance* AnimInstance = SeaCreature->GetMesh() ? SeaCreature->GetMesh()->GetAnimInstance() : nullptr;
	if (AnimInstance == nullptr) return EBTNodeResult::Failed;

	AMyRobo* Target = Cast<AMyRobo>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ASeaCreatureAIController::TargetActorKey));
	if (Target == nullptr) 	return EBTNodeResult::Failed;

	MyOwnerComp = &OwnerComp;

	AnimInstance->OnMontageEnded.AddDynamic(this, &UTask_Attack::OnMontageEnded);

	SeaCreature->Attack(Target);

	return EBTNodeResult::InProgress;
}

void UTask_Attack::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (MyOwnerComp.IsValid() && !bInterrupted)
	{
		FinishLatentTask(*MyOwnerComp.Get(), EBTNodeResult::Succeeded);
	}
}

void UTask_Attack::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);

	ASeaCreature* SeaCreature = Cast<ASeaCreature>(OwnerComp.GetAIOwner()->GetPawn());
	if (SeaCreature)
	{
		if (UAnimInstance* Anim = SeaCreature->GetMesh()->GetAnimInstance())
		{
			Anim->OnMontageEnded.RemoveDynamic(this,&UTask_Attack::OnMontageEnded);
		}
	}
	MyOwnerComp.Reset();
}

