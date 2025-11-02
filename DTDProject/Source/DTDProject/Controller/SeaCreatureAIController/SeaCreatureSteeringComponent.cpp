// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/SeaCreatureAIController/SeaCreatureSteeringComponent.h"
#include "SeaCreature/SeaCreature.h"
#include "GameFramework/FloatingPawnMovement.h"


USeaCreatureSteeringComponent::USeaCreatureSteeringComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	//HomeSphere = CreateDefaultSubobject<USphereComponent>(TEXT("HomeSphere"));
	//HomeSphere->SetSphereRadius(WanderRadius);
	////HomeSphere->SetupAttachment(RootComponent);
	//HomeSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void USeaCreatureSteeringComponent::InitParams(float InWanderRadius, float InSlowRadius)
{
	WanderRadius = InWanderRadius;
	SlowRadius = InSlowRadius;
}

FVector USeaCreatureSteeringComponent::ComputeSeekDir(const FVector& TargetLocation) const
{
	return Seek(TargetLocation);
}

FVector USeaCreatureSteeringComponent::ComputeFleeDir(const FVector& FromLocation) const
{
	return Flee(FromLocation);
}

FVector USeaCreatureSteeringComponent::ComputeWanderDir(float DeltaTime)
{
	return Wander(DeltaTime);
}

FVector USeaCreatureSteeringComponent::ComputeAvoidanceDir() const
{
	return ObstacleAvoidance();
}

FVector USeaCreatureSteeringComponent::ComputeApplyMoveInput(float DeltaSeconds)
{
	return ApplyMoveInput(DeltaSeconds);
}


// Called when the game starts
void USeaCreatureSteeringComponent::BeginPlay()
{
	Super::BeginPlay();

	SeaCreatureOwner = Cast<ASeaCreature>(GetOwner());

	Home = GetOwner()->GetActorLocation();

}

FVector USeaCreatureSteeringComponent::Seek(const FVector& Target) const
{
	return (Target - SeaCreatureOwner->GetActorLocation()).GetSafeNormal();
}

FVector USeaCreatureSteeringComponent::Arrive(const FVector& RandWanderPoint) const
{
	const FVector to = RandWanderPoint - SeaCreatureOwner->GetActorLocation();
	const float d = to.Size();
	if (d < 1.f) return FVector::ZeroVector;
	const float scale = FMath::Clamp(d / SlowRadius, 0.15f, 1.f);
	return to * (1.f / FMath::Max(d, 1.f)) * scale;
}

FVector USeaCreatureSteeringComponent::Flee(const FVector& TargetLocation) const
{
	FVector v = SeaCreatureOwner->GetActorLocation()- TargetLocation;
	return v.GetSafeNormal();
}

FVector USeaCreatureSteeringComponent::Wander(float DeltaTime)
{
	static float Timer = 0.f; Timer += DeltaTime;
	if (Timer > 2.f || FVector::DistSquared(SeaCreatureOwner->GetActorLocation(), CurrentWanderTarget) < 150.f * 150.f)
	{
		Timer = 0.f;
		// 홈 중심 구형 범위 내 랜덤 포인트
		CurrentWanderTarget = Home + FMath::VRand() * WanderRadius;
	}
	return Arrive(CurrentWanderTarget);
}

FVector USeaCreatureSteeringComponent::ObstacleAvoidance() const
{
	const float Probe = 300.f, Radius = 50.f;
	const FVector P = SeaCreatureOwner->GetActorLocation();
	const FVector Fwd = SeaCreatureOwner->GetVelocity().IsNearlyZero() ? SeaCreatureOwner->GetActorForwardVector() : SeaCreatureOwner->GetVelocity().GetSafeNormal();

	TArray<FVector> dirs;
	dirs.Add(Fwd);
	dirs.Add(Fwd.RotateAngleAxis(30.f, SeaCreatureOwner->GetActorRightVector()));
	dirs.Add(Fwd.RotateAngleAxis(-30.f, SeaCreatureOwner->GetActorRightVector()));
	dirs.Add(Fwd.RotateAngleAxis(30.f, SeaCreatureOwner->GetActorUpVector()));
	dirs.Add(Fwd.RotateAngleAxis(-30.f, SeaCreatureOwner->GetActorUpVector()));

	FHitResult hit;
	for (const FVector& d : dirs)
	{
		const FVector start = P + d * 30.f;
		const FVector end = start + d * Probe;
		if (GetWorld()->SweepSingleByChannel(hit, start, end, FQuat::Identity, ECC_WorldStatic,
			FCollisionShape::MakeSphere(Radius)))
		{
			return FVector::VectorPlaneProject(d, hit.Normal).GetSafeNormal() * 0.8f;
		}
	}
	return FVector::ZeroVector;
}

FVector USeaCreatureSteeringComponent::ApplyMoveInput(float DeltaSeconds)
{
	FVector Dir = FVector::ZeroVector;
	Dir += Wander(DeltaSeconds);
	Dir += ObstacleAvoidance();
	return Dir.GetSafeNormal();
}


// Called every frame
void USeaCreatureSteeringComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

