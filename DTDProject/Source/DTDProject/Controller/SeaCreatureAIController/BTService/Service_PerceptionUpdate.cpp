// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/SeaCreatureAIController/BTService/Service_PerceptionUpdate.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "MyRobo/MyRobo.h"
#include "BehaviorTree/BlackboardComponent.h"

UService_PerceptionUpdate::UService_PerceptionUpdate()
{
	NodeName = TEXT("Perception Update");
	bNotifyBecomeRelevant = true;
	bNotifyCeaseRelevant = false;
}

void UService_PerceptionUpdate::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AIController = OwnerComp.GetAIOwner();
	APawn* Pawn = AIController->GetPawn();
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp) return;

	UAIPerceptionComponent* PerceptionComp = AIController->FindComponentByClass<UAIPerceptionComponent>();
	if (PerceptionComp) return;

	TArray<AActor*> Sensed;
	PerceptionComp->GetCurrentlyPerceivedActors(nullptr, Sensed);

	AMyRobo* Robo = nullptr;
	float Dist = -1.f;

	for (AActor* Actor : Sensed)
	{
		Robo = Cast<AMyRobo>(Actor);
		if (Robo)
		{
			Dist = FVector::Dist(Pawn->GetActorLocation(), Robo->GetActorLocation());
			break;
		}
	}

	if (Robo)
	{
		BlackboardComp->SetValueAsObject(TEXT("TargetActor"), Robo);
		BlackboardComp->SetValueAsBool(TEXT("HasThreat"), true);
		BlackboardComp->SetValueAsFloat(TEXT("DistanceToTarget"), Dist);
	}
	else
	{
		BlackboardComp->ClearValue(TEXT("TargetActor"));
		BlackboardComp->SetValueAsBool(TEXT("HasThreat"), false);
		BlackboardComp->SetValueAsFloat(TEXT("DistanceToTarget"), -1.f);
	}

}
