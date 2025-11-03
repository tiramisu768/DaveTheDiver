// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
//#include "Components/SphereComponent.h"
#include "SeaCreature/SeaCreatureStateType.h"
#include "SeaCreatureSteeringComponent.generated.h"

class ASeaCreature;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DTDPROJECT_API USeaCreatureSteeringComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USeaCreatureSteeringComponent();

	void InitParams(float InWanderRadius, float InSlowRadius);

	FVector ComputeSeekDir(const FVector& TargetLocation) const;
	FVector ComputeFleeDir(const FVector& TargetLocation) const;
	FVector ComputeWanderDir(float DeltaTime);
	FVector ComputeAvoidanceDir() const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Steering")
	FVector Home; //물고기 스폰위치
	FVector CurrentWanderTarget; //RandWanderPoint(홈 중심 구형 범위 내 랜덤 포인트)
	float WanderRadius;
	float SlowRadius;

	ASeaCreature* SeaCreatureOwner = nullptr;
	//UPROPERTY(VisibleAnywhere, Category = "AI|Home")
	//USphereComponent* HomeSphere;

	FVector Seek(const FVector& Target) const; //로봇 쫓아가기
	FVector Arrive(const FVector& RandWanderPoint) const; //랜덤배회 위치로 도달
	FVector Flee(const FVector& TargetLocation) const; //로봇으로부터 도망가기
	FVector Wander(float DeltaTime); //래덤위치로 배회
	FVector ObstacleAvoidance() const; //장애물인지 후 피하기

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
