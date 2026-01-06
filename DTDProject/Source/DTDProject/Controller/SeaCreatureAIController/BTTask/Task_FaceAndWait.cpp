// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/SeaCreatureAIController/BTTask/Task_FaceAndWait.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "SeaCreature/SeaCreature.h"
#include "Controller/SeaCreatureAIController/SeaCreatureAIController.h"

UTask_FaceAndWait::UTask_FaceAndWait()
{
    NodeName = TEXT("Face And Wait");
    bNotifyTick = true;
}

EBTNodeResult::Type UTask_FaceAndWait::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp) return EBTNodeResult::Failed;

    const float CooldownAfterAttack = 1.0f;
    float LastAttack = BlackboardComp->GetValueAsFloat(ASeaCreatureAIController::LastAttackEndTimeKey);
    float Now = OwnerComp.GetWorld()->GetTimeSeconds();
    if (LastAttack > 0.f && (Now - LastAttack) < CooldownAfterAttack)
    {
        return EBTNodeResult::Failed;
    }

    AActor* Target = Cast<AActor>(BlackboardComp->GetValueAsObject(ASeaCreatureAIController::TargetActorKey));
    if (!Target) return EBTNodeResult::Failed;

    FBTFaceAndWaitMemory* MyMemory = (FBTFaceAndWaitMemory*)NodeMemory;
    MyMemory->RememberedTarget = Target;

    BlackboardComp->SetValueAsFloat(ASeaCreatureAIController::FaceStartTimeKey, Now);
    return EBTNodeResult::InProgress;
}

void UTask_FaceAndWait::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

    FBTFaceAndWaitMemory* MyMemory = (FBTFaceAndWaitMemory*)NodeMemory;
    AActor* TargetActor = MyMemory->RememberedTarget.Get();

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    ASeaCreature* SeaCreature = Cast<ASeaCreature>(OwnerComp.GetAIOwner()->GetPawn());

    if (BlackboardComp == nullptr || SeaCreature == nullptr || SeaCreature->SteeringComp == nullptr)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    if (TargetActor)
    {
        FVector Dir = TargetActor->GetActorLocation() - SeaCreature->GetActorLocation();
        FRotator Desired = Dir.Rotation();
        FRotator NewRot = FMath::RInterpTo(SeaCreature->GetActorRotation(), FRotator(0.f, Desired.Yaw, 0.f), DeltaSeconds, RotationSpeed);
        SeaCreature->SetActorRotation(NewRot);
    }

    const float StartTime = BlackboardComp->GetValueAsFloat(ASeaCreatureAIController::FaceStartTimeKey);
    const float Elapsed = OwnerComp.GetWorld()->GetTimeSeconds() - StartTime;
    if (Elapsed >= WaitTime)
    {
        if (TargetActor)
        {
            BlackboardComp->SetValueAsVector(ASeaCreatureAIController::ChaseTargetLocationKey, TargetActor->GetActorLocation());
            BlackboardComp->SetValueAsBool(ASeaCreatureAIController::IsChargingKey, true);
        }

        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }

}

void UTask_FaceAndWait::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type Result)
{
    UBlackboardComponent* BlackboarComp = OwnerComp.GetBlackboardComponent();
    if (BlackboarComp)
    {
        BlackboarComp->ClearValue(ASeaCreatureAIController::FaceStartTimeKey);
    }

    Super::OnTaskFinished(OwnerComp, NodeMemory, Result);
}
