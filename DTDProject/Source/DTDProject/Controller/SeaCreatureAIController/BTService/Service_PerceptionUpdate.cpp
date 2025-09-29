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
	APawn* SeaCreature = AIController->GetPawn();
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!AIController || !SeaCreature || !BlackboardComp) return;

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
			break;
	}

	 const bool bHadThreat = BlackboardComp->GetValueAsBool(TEXT("HasThreat"));

	if (Robo)
	{
		Dist = FVector::Dist(SeaCreature->GetActorLocation(), Robo->GetActorLocation());
		BlackboardComp->SetValueAsFloat(TEXT("DistanceToTarget"), Dist);

		const float Enter = BlackboardComp->GetValueAsFloat(TEXT("ReactionEnterDist")); //100
		const float Exit = BlackboardComp->GetValueAsFloat(TEXT("ReactionExitDist")); //200
		//공격물고기가 attack준비를 해야되는 곳?
		bool bThreat = bHadThreat;

		if (!bHadThreat)
		{
			//기존위협이 없었고 Flee진입구간에 들어오면 새 위협이 true
			if (Enter > 0.f && Dist <= Enter) bThreat = true;
		}
		else
		{
			//기존위협이 있었고 Flee진출구간에서 벗어나면 새 위협이 false
			if (Exit > 0.f && Dist > Exit) bThreat = false;
		}

		if (bThreat)
		{
			//로보가 근처에 있음
			BlackboardComp->SetValueAsObject(TEXT("TargetActor"), Robo);
			BlackboardComp->SetValueAsBool(TEXT("HasThreat"), true);
		}
		else
		{
			//로보가 근처에 없음
			BlackboardComp->ClearValue(TEXT("TargetActor"));
			BlackboardComp->SetValueAsBool(TEXT("HasThreat"), false);
		}

	}
	else
	{
		//perception 전혀 못 함
		BlackboardComp->ClearValue(TEXT("TargetActor"));
		BlackboardComp->SetValueAsBool(TEXT("HasThreat"), false);
		BlackboardComp->SetValueAsFloat(TEXT("DistanceToTarget"), -1.f);
	}

	//Home 갱신
	FVector HomeLoc = BlackboardComp->GetValueAsVector(TEXT("HomeLocation"));
	const FVector SeaCreatureLoc = SeaCreature->GetActorLocation();

	// 거리 계산 + 무한대/NaN 가드
	const float DistHome = FVector::Dist(SeaCreatureLoc, HomeLoc);
	BlackboardComp->SetValueAsFloat(TEXT("DistanceFromHome"),FMath::IsFinite(DistHome) ? DistHome : 0.f);

	// 디버그 스피어 그리기 (반지름 = DistanceFromHome)
	if (GEngine && SeaCreature->GetWorld())
	{
		DrawDebugSphere(
			SeaCreature->GetWorld(),
			HomeLoc,                     // 중심: HomeLocation
			DistHome,                 // 반지름
			16,                       // 세그먼트 수
			FColor::Purple,            // 색상
			false,                    // 영구 표시 여부 (false면 잠깐만 보임)
			0.1f                      // 지속 시간 (초)
		);
	}

	FVector LastRoboPos;
	float UpdateThreshold = 300.f;
	//HomeLocation_Dynamic 갱신
	if (FVector::DistSquared(Robo->GetActorLocation(), LastRoboPos) > FMath::Square(UpdateThreshold))
	{
		FVector HomeLocation_Dynamic = Robo->GetActorLocation() + Robo->GetActorForwardVector() * 500;
		BlackboardComp->SetValueAsVector(TEXT("HomeLocation_Dynamic"), HomeLocation_Dynamic);
		LastRoboPos = Robo->GetActorLocation();

		// 디버그 스피어 그리기 (반지름 = DistanceFromHome)
		if (GEngine && SeaCreature->GetWorld())
		{
			DrawDebugSphere(
				SeaCreature->GetWorld(),
				HomeLocation_Dynamic,     // 중심: HomeLocation_Dynamic
				DistHome,                 // 반지름
				16,                       // 세그먼트 수
				FColor::Orange,            // 색상
				false,                    // 영구 표시 여부 (false면 잠깐만 보임)
				0.1f                      // 지속 시간 (초)
			);
		}
	}



}
