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
	if (!PerceptionComp) return;

	//로봇 갱신
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

	//Home 갱신
	FVector Home = BlackboardComp->GetValueAsVector(TEXT("HomeLocation"));

	const FVector Loc = Pawn->GetActorLocation();

	UE_LOG(LogTemp, Warning, TEXT("Home=%s  Loc=%s"), *Home.ToString(), *Loc.ToString());
	UE_LOG(LogTemp, Warning, TEXT("Home NaN? %d  Loc NaN? %d"),
		Home.ContainsNaN(), Loc.ContainsNaN());


	float DistHome = FVector::Dist(Pawn->GetActorLocation(), Home);
	BlackboardComp->SetValueAsFloat(TEXT("DistanceFromHome"), DistHome);
}
