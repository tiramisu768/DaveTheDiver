// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/SeaCreatureAIController/BTTask/Task_Flee.h"
#include "Controller/SeaCreatureAIController/SeaCreatureAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "SeaCreature/SeaCreature.h"
#include "Controller/SeaCreatureAIController/SeaCreatureSteeringComponent.h"
#include "SeaCreature/SeaCreatureStateType.h"

void UTask_Flee::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* Owner = OwnerComp.GetAIOwner();
	ASeaCreature* SeaCreature = nullptr;

	if (nullptr != Owner)
		SeaCreature = Cast<ASeaCreature>(Owner->GetPawn());

	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();

	AActor* Target = Cast<AActor>(BlackboardComponent->GetValueAsObject("TargetActor"));

	//µµ¸Á¼º°ø Á¶°Ç : Å¸°ÙÀ» ÀÒ¾î¹ö¸², °Å¸®°¡ ¸Ö¾îÁü fleedist? losetargetdist?
	if (nullptr == SeaCreature || nullptr == Target)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	FVector dir = SeaCreature->SteeringComp->ComputeFleeDir(Target->GetActorLocation());
	dir += SeaCreature->SteeringComp->ComputeAvoidanceDir();
	dir.Z = 0.f;
	SeaCreature->SteeringComp->ComputeApplyMoveInput(dir.GetSafeNormal());
}
