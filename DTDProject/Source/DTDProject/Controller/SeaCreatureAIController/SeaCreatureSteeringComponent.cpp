// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/SeaCreatureAIController/SeaCreatureSteeringComponent.h"
#include "SeaCreature/SeaCreature.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameModeBase/CharacterGameModeBase/CharacterGameModeBase.h"
#include "Kismet/GameplayStatics.h"


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

FVector USeaCreatureSteeringComponent::ComputeChaseDir(const FVector& TargetLocation) const
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

	SeaCreatureOwner = Cast<ASeaCreature>(GetOwner());
	if(SeaCreatureOwner)
	{
		Home = GetOwner()->GetActorLocation();
	}

	UpdateWanderTarget();

	GetWorld()->GetTimerManager().SetTimer(
		WanderTargetUpdateTimerHandle,
		this,
		&USeaCreatureSteeringComponent::UpdateWanderTarget,
		2.0f,
		true);
}

void USeaCreatureSteeringComponent::UpdateWanderTarget()
{
	if (SeaCreatureOwner == nullptr) return;

	const FVector ForwardVec = SeaCreatureOwner->GetActorForwardVector();

	FVector RandomDirection = ForwardVec.RotateAngleAxis(FMath::FRandRange(-45.f, 45.f), FVector::UpVector);

	RandomDirection = RandomDirection.RotateAngleAxis(FMath::FRandRange(-90.f, 90.f), SeaCreatureOwner->GetActorRightVector());

	FVector DesiredTarget = SeaCreatureOwner->GetActorLocation() + RandomDirection.GetSafeNormal() * WanderRadius;

	const float DistFromHome = FVector::Dist(SeaCreatureOwner->GetActorLocation(), Home);
	if (DistFromHome > WanderRadius)
	{
		FVector ToHomeDir = (Home - SeaCreatureOwner->GetActorLocation()).GetSafeNormal();
		
		DesiredTarget = SeaCreatureOwner->GetActorLocation() + (RandomDirection + ToHomeDir).GetSafeNormal() * WanderRadius;
	}

	CurrentWanderTarget = DesiredTarget;

	if (ACharacterGameModeBase* GameMode = Cast<ACharacterGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		if (CurrentWanderTarget.Z > GameMode->LimitZ)
		{
			CurrentWanderTarget.Z = GameMode->LimitZ;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Not Found GameMode in SteeringComponent"));
	}
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
	FVector FleeDir = SeaCreatureOwner->GetActorLocation()- TargetLocation;
	FleeDir.Z = 0; //수평으로만 도망가도록 Z값을 무시한다
	return FleeDir.GetSafeNormal();
}

FVector USeaCreatureSteeringComponent::Wander(float DeltaTime)
{
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
			FVector AvoidanceDir = FVector::VectorPlaneProject(d, hit.Normal).GetSafeNormal();
			if (AvoidanceDir.Z > 0)
			{
				AvoidanceDir.Z = 0;
			}
			return AvoidanceDir;
		}
	}
	return FVector::ZeroVector;
}

// Called every frame
void USeaCreatureSteeringComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

