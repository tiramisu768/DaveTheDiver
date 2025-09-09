// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/SeaCreatureAIController/SeaCreatureSteeringComponent.h"


// Sets default values for this component's properties
USeaCreatureSteeringComponent::USeaCreatureSteeringComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//HomeSphere = CreateDefaultSubobject<USphereComponent>(TEXT("HomeSphere"));
	//HomeSphere->SetSphereRadius(WanderRadius);
	////HomeSphere->SetupAttachment(RootComponent);
	//HomeSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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


// Called when the game starts
void USeaCreatureSteeringComponent::BeginPlay()
{
	Super::BeginPlay();

	Home = GetOwner()->GetActorLocation();
	
}

FVector USeaCreatureSteeringComponent::Seek(const FVector& Target) const
{
	GEngine->AddOnScreenDebugMessage(-7, 3.0f, FColor::Purple, TEXT("Seek"));
	return (Target - GetActorLocation()).GetSafeNormal();
}

FVector USeaCreatureSteeringComponent::Arrive(const FVector& Target) const
{
	const FVector to = Target - GetActorLocation();
	const float d = to.Size();
	if (d < 1.f) return FVector::ZeroVector;
	const float scale = FMath::Clamp(d / SlowRadius, 0.15f, 1.f);
	return to * (1.f / FMath::Max(d, 1.f)) * scale;
}

FVector USeaCreatureSteeringComponent::Flee(const FVector& FromLocation) const
{
	GEngine->AddOnScreenDebugMessage(-7, 3.0f, FColor::Purple, TEXT("Flee"));
	return (GetActorLocation() - From).GetSafeNormal();
}

FVector USeaCreatureSteeringComponent::Wander(float DeltaTime)
{
	GEngine->AddOnScreenDebugMessage(-7, 3.0f, FColor::Purple, TEXT("Wander"));
	static float Timer = 0.f; Timer += dt;
	if (Timer > 2.f || FVector::DistSquared(GetActorLocation(), CurrentWanderTarget) < 150.f * 150.f)
	{
		Timer = 0.f;
		// 홈 중심 구형 범위 내 랜덤 포인트
		CurrentWanderTarget = Home + FMath::VRand() * WanderRadius;
	}
	return Arrive(CurrentWanderTarget);
}

FVector USeaCreatureSteeringComponent::ObstacleAvoidance() const
{
	GEngine->AddOnScreenDebugMessage(-7, 3.0f, FColor::Purple, TEXT("ObstacleAvoidance"));
	const float Probe = 300.f, Radius = 50.f;
	const FVector P = GetActorLocation();
	const FVector Fwd = GetVelocity().IsNearlyZero() ? GetActorForwardVector() : GetVelocity().GetSafeNormal();

	TArray<FVector> dirs;
	dirs.Add(Fwd);
	dirs.Add(UKismetMathLibrary::RotateAngleAxis(Fwd, 30.f, GetActorRightVector()));
	dirs.Add(UKismetMathLibrary::RotateAngleAxis(Fwd, -30.f, GetActorRightVector()));
	dirs.Add(UKismetMathLibrary::RotateAngleAxis(Fwd, 30.f, GetActorUpVector()));
	dirs.Add(UKismetMathLibrary::RotateAngleAxis(Fwd, -30.f, GetActorUpVector()));

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

void USeaCreatureSteeringComponent::ApplyMoveInput(const FVector& Dir)
{
	if (!Dir.IsNearlyZero())
	{
		AddMovementInput(Dir, 1.f);
		GetCharacterMovement()->MaxFlySpeed = MaxSpeed;
	}
}


// Called every frame
void USeaCreatureSteeringComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

