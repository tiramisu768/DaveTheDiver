// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/SeaCreatureSpawner/SeaCreatureSpawner.h"

// Sets default values
ASeaCreatureSpawner::ASeaCreatureSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASeaCreatureSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASeaCreatureSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

