// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/SeaCreatureAIController/BTTask/Task_ReturnHome.h"
#include "Controller/SeaCreatureAIController/SeaCreatureAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "SeaCreature/SeaCreature.h"
#include "Controller/SeaCreatureAIController/SeaCreatureSteeringComponent.h"
#include "SeaCreature/SeaCreatureStateType.h"

void UTask_ReturnHome::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    AAIController* Owner = OwnerComp.GetAIOwner();
    UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
    ASeaCreature* SeaCreature = Cast<ASeaCreature>(Owner->GetPawn());

    auto* Steer = SeaCreature->SteeringComp;
    if (!Owner || !BlackboardComponent || !SeaCreature || !Steer) { FinishLatentTask(OwnerComp, EBTNodeResult::Failed); return; } //비정상 상황

    const FVector Home = BlackboardComponent->GetValueAsVector("HomeLocation");
    const FVector dir = (Steer->ComputeSeekDir(Home) + Steer->ComputeAvoidanceDir()).GetSafeNormal();
    SeaCreature->SteeringComp->ComputeApplyMoveInput(dir.GetSafeNormal(), ESeaCreatureState::ReturnHome);

    //const float tol = BlackboardComponent->GetValueAsFloat("HomeArriveTolerance"); // 예: 150.f
    //const float d = FVector::Dist(SeaCreature->GetActorLocation(), Home);
    //if (d <= tol) 
    //{
    //    FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded); // → Selector가 Wander로 폴백
    //}
}
