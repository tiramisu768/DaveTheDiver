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

    AActor* Target = Cast<AActor>(BlackboardComp->GetValueAsObject(ASeaCreatureAIController::TargetActorKey));
    if (!Target) return EBTNodeResult::Failed;

    BlackboardComp->SetValueAsFloat(TEXT("FaceStartTime"), OwnerComp.GetWorld()->GetTimeSeconds());
    return EBTNodeResult::InProgress;
}

void UTask_FaceAndWait::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    AActor* Target = Cast<AActor>(BlackboardComp->GetValueAsObject(ASeaCreatureAIController::TargetActorKey));
    ASeaCreature* SeaCreature = Cast<ASeaCreature>(OwnerComp.GetAIOwner()->GetPawn());
    if (!BlackboardComp||!SeaCreature || !Target)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return; 
    }

    const FVector SeaCreatureLoc = SeaCreature->GetActorLocation();
    FVector TargetLoc = Target->GetActorLocation();
    TargetLoc.Z = SeaCreatureLoc.Z;
    FVector Dir = TargetLoc - SeaCreatureLoc;
    if (!Dir.IsNearlyZero())
    {
        FRotator Desired = Dir.Rotation();
        FRotator Current = SeaCreature->GetActorRotation();
        FRotator NewRot = FMath::RInterpTo(Current, FRotator(0.f, Desired.Yaw, 0.f), DeltaSeconds, RotationSpeed);
        SeaCreature->SetActorRotation(NewRot);
    }

    const float StartTime = BlackboardComp->GetValueAsFloat(TEXT("FaceStartTime"));
    const float Elapsed = OwnerComp.GetWorld()->GetTimeSeconds() - StartTime;
    if (Elapsed >= WaitTime)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
}

void UTask_FaceAndWait::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type Result)
{
}
