// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/SeaCreatureAIController/SeaCreatureSteeringComponent.h"


// Sets default values for this component's properties
USeaCreatureSteeringComponent::USeaCreatureSteeringComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
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
	return FVector();
}

FVector USeaCreatureSteeringComponent::Flee(const FVector& FromLocation) const
{
	GEngine->AddOnScreenDebugMessage(-7, 3.0f, FColor::Purple, TEXT("Flee"));
	return FVector();
}

FVector USeaCreatureSteeringComponent::Wander(float DeltaTime)
{
	GEngine->AddOnScreenDebugMessage(-7, 3.0f, FColor::Purple, TEXT("Wander"));
	return FVector();
}

FVector USeaCreatureSteeringComponent::ObstacleAvoidance() const
{
	GEngine->AddOnScreenDebugMessage(-7, 3.0f, FColor::Purple, TEXT("ObstacleAvoidance"));
	return FVector();
}


// Called every frame
void USeaCreatureSteeringComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

