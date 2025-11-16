// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/SeaCreatureAIController/BTDecorator/Decorator_AttackInRange.h"
#include "Controller/SeaCreatureAIController/SeaCreatureAIController.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SeaCreature/SeaCreature.h"


UDecorator_AttackInRange::UDecorator_AttackInRange()
{
	NodeName = TEXT("Attack In Range");
}

bool UDecorator_AttackInRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	ASeaCreature* SeaCreature = Cast<ASeaCreature>(OwnerComp.GetAIOwner()->GetPawn());
	if (BlackboardComp == nullptr || SeaCreature == nullptr)
	{
		return false;
	}

	AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(ASeaCreatureAIController::TargetActorKey));
	if (TargetActor == nullptr)
	{
		return false;
	}

	const float AttackRange = SeaCreature->GetData()->AttackRange;
	const float DistanceToTarget = FVector::Dist(SeaCreature->GetActorLocation(), TargetActor->GetActorLocation());

	const bool bIsInAttackRange = DistanceToTarget <= AttackRange;

	// 디버그 메시지 추가
	if (GEngine)
	{
		FString DebugMsg = FString::Printf(TEXT("AttackInRange: Dist: %.1f / Range: %.1f -> %s"),
			DistanceToTarget,
			AttackRange,
			bIsInAttackRange ? TEXT("TRUE") : TEXT("FALSE"));

		// 메시지 키, 유지 시간, 색상, 실제 메시지
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, DebugMsg);
	}

	return bIsInAttackRange;
}
