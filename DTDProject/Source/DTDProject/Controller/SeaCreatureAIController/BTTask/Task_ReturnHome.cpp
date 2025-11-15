// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/SeaCreatureAIController/BTTask/Task_ReturnHome.h"
#include "Controller/SeaCreatureAIController/SeaCreatureAIController.h"
#include "Controller/SeaCreatureAIController/SeaCreatureSteeringComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "SeaCreature/SeaCreature.h"
#include "SeaCreature/SeaCreatureStateType.h"
#include "GameFramework/FloatingPawnMovement.h"

UTask_ReturnHome::UTask_ReturnHome()
{
    bNotifyTick = true;
    NodeName = TEXT("Return Home");
}

EBTNodeResult::Type UTask_ReturnHome::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, TEXT("RETURNHOME TASK STARTED!"));
    ASeaCreature* SeaCreature = Cast<ASeaCreature>(OwnerComp.GetAIOwner()->GetPawn());
    const FSeaCreatureData* FishData = SeaCreature->GetData();

    SeaCreature->MovementComponent->MaxSpeed = FishData->ReturnSpeed;
    SeaCreature->MovementComponent->Acceleration = FishData->Acceleration;

    return EBTNodeResult::InProgress;
}

void UTask_ReturnHome::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    ASeaCreature* SeaCreature = Cast<ASeaCreature>(OwnerComp.GetAIOwner()->GetPawn());
    if (BlackboardComp==nullptr || SeaCreature == nullptr || SeaCreature->SteeringComp == nullptr)
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

    const FVector HomeLocation = BlackboardComp->GetValueAsVector(ASeaCreatureAIController::HomeLocationKey);
    const float DistToHome = FVector::Dist(SeaCreature->GetActorLocation(), HomeLocation);

    if (DistToHome < 100.f)
    {
        BlackboardComp->SetValueAsBool(ASeaCreatureAIController::IsFarFromHomeKey, false);

        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        return;
    }

    FVector Dir = SeaCreature->SteeringComp->ComputeChaseDir(HomeLocation);
    Dir += SeaCreature->SteeringComp->ComputeAvoidanceDir();
    Dir.Normalize();

    if (!Dir.IsNearlyZero())
    {
        SeaCreature->AddMovementInput(Dir,FishData->ReturnSpeed);
        FRotator TargetRotation = Dir.Rotation();
        SeaCreature->SetActorRotation(FMath::RInterpTo(SeaCreature->GetActorRotation(), TargetRotation, DeltaSeconds, 2.0f));
    }
}
