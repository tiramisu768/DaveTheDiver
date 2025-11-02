// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/SeaCreatureAIController/BTTask/Task_ReturnHome.h"
#include "Controller/SeaCreatureAIController/SeaCreatureAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "SeaCreature/SeaCreature.h"
#include "Controller/SeaCreatureAIController/SeaCreatureSteeringComponent.h"
#include "SeaCreature/SeaCreatureStateType.h"
#include "GameFramework/FloatingPawnMovement.h"

UTask_ReturnHome::UTask_ReturnHome()
{
    bNotifyTick = true;
}

EBTNodeResult::Type UTask_ReturnHome::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
    return EBTNodeResult::InProgress;
}

void UTask_ReturnHome::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

    ASeaCreature* SeaCreature = Cast<ASeaCreature>(OwnerComp.GetAIOwner()->GetPawn());
    if (SeaCreature == nullptr || SeaCreature->SteeringComp == nullptr || SeaCreature->MovementComponent == nullptr)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    const FSeaCreatureData* FishData = SeaCreature->GetData();
    if (FishData == nullptr)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    //FVector HomeLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(TEXT("HomeLocation"));
    //float DistanceToHome = FVector::Dist(SeaCreature->GetActorLocation(), HomeLocation);
    
    // UBTTask_BlackboardBase를 사용하므로, GetValueAsVector로 직접 키 이름을 쓰는 대신
    // 설정된 블랙보드 키에서 값을 가져옵니다.
    FVector HomeLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());
    float DistanceToHome = FVector::Dist(SeaCreature->GetActorLocation(), HomeLocation);

    if (DistanceToHome < 100.0f)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        return;
    }

    FVector Dir = SeaCreature->SteeringComp->ComputeSeekDir(HomeLocation);
    Dir += SeaCreature->SteeringComp->ComputeAvoidanceDir();
    Dir.Normalize();

    if (!Dir.IsNearlyZero())
    {
        SeaCreature->MovementComponent->MaxSpeed = FishData->ReturnSpeed;
        SeaCreature->AddMovementInput(Dir);
        FRotator TargetRotation = Dir.Rotation();
        SeaCreature->SetActorRotation(FMath::RInterpTo(SeaCreature->GetActorRotation(), TargetRotation, DeltaSeconds, 2.0f));
    }

}
