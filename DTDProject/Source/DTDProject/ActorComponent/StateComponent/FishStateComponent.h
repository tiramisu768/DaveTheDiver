// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponent/StateComponent/StateComponent.h"
//#include "Engine/DataTable.h"
#include "FishStateComponent.generated.h"

//// 물고기 DataTable Row
//USTRUCT(BlueprintType)
//struct FFishStats : public FTableRowBase
//{
//    GENERATED_BODY()
//
//    UPROPERTY(EditAnywhere, BlueprintReadWrite) float MaxHP = 10.f;
//    UPROPERTY(EditAnywhere, BlueprintReadWrite) float MoveSpeed = 300.f;
//    UPROPERTY(EditAnywhere, BlueprintReadWrite) float DamageMultiplier = 1.f;
//    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool  bIsAggressive = false;
//    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool  bIsBoss = false;
//    // … 필요한 특성들 추가
//};
/**
 * 
 */
UCLASS()
class DTDPROJECT_API UFishStateComponent : public UStateComponent
{
	GENERATED_BODY()
    
protected:
    virtual void BeginPlay() override;

public:
    UFishStateComponent();

    // ====== DataTable 연결 ======
    //UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State | Fish")
    //UDataTable* FishDataTable;

    //UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State | Fish")
    //FName SpeciesId; // SmallFish, MediumFish, PredatorFish 등 Row Key

    //UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Fish")
    //FFishStats Stats; // 현재 물고기 종의 스탯 캐싱

    virtual void TakeDamage(float Amount) override;
    void LoadStatsFromDataTable();
};
