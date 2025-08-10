// Fill out your copyright notice in the Description page of Project Settings.


#include "SeaCreature/SeaCreature.h"

// Sets default values
ASeaCreature::ASeaCreature()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StateComponent = CreateDefaultSubobject<UStateComponent>(TEXT("StateComponent"));
}

// Called when the game starts or when spawned
void ASeaCreature::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASeaCreature::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASeaCreature::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

