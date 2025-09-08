// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SeaCreatureSteeringComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DTDPROJECT_API USeaCreatureSteeringComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USeaCreatureSteeringComponent();

	FVector ComputeSeekDir(const FVector& TargetLocation) const;
	FVector ComputeFleeDir(const FVector& FromLocation) const;
	FVector ComputeWanderDir(float DeltaTime);
	FVector ComputeAvoidanceDir() const;

	void SetHome(const FVector& Location) { Home = Location; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	FVector Home;
	FVector CurrentWanderTarget;
	float WanderRadius = 800.f;
	float SlowRadius = 300.f;

	FVector Seek(const FVector& TargetLocation) const;
	FVector Flee(const FVector& FromLocation) const;
	FVector Wander(float DeltaTime);
	FVector ObstacleAvoidance() const;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
