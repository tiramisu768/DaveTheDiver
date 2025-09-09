// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
//#include "Components/SphereComponent.h"
#include "SeaCreatureSteeringComponent.generated.h"

class ASeaCreature;

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
	void ComputeApplyMoveInput(const FVector& Dir);

	void SetHome(const FVector& Location) { Home = Location; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	FVector Home;
	FVector CurrentWanderTarget;
	float WanderRadius = 800.f;
	float SlowRadius = 300.f;
	float MaxSpeed = 200.f;
	ASeaCreature* SeaCreatureOwner = nullptr;
	//UPROPERTY(VisibleAnywhere, Category = "AI|Home")
	//USphereComponent* HomeSphere;

	FVector Seek(const FVector& Target) const;
	FVector Arrive(const FVector& Target) const;
	FVector Flee(const FVector& FromLocation) const;
	FVector Wander(float DeltaTime);
	FVector ObstacleAvoidance() const;

	void ApplyMoveInput(const FVector& Dir);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
