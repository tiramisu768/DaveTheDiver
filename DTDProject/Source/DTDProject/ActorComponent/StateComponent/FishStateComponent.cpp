// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/StateComponent/FishStateComponent.h"

UFishStateComponent::UFishStateComponent()
{
}

void UFishStateComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UFishStateComponent::TakeDamage(float Amount)
{
	Super::TakeDamage(Amount);
}

void UFishStateComponent::Heal(float HealAmount)
{
	Super::Heal(HealAmount);
}

void UFishStateComponent::LoadStatsFromDataTable()
{
}
