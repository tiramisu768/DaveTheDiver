// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/StateComponent/FishStateComponent.h"

UFishStateComponent::UFishStateComponent()
{
}

void UFishStateComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UFishStateComponent::TakeDamage(float Amount,const FHitResult& HitResult)
{
	Super::TakeDamage(Amount,HitResult);
}

// FSeaCreatureData로부터 스탯(최대 체력 등)을 로드하는 역할을 담당하도록 설계
void UFishStateComponent::LoadStatsFromDataTable()
{
}
