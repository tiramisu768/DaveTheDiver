// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SeaCreatureData.generated.h"

USTRUCT(BlueprintType)
struct FSeaCreatureStats : public FTableRowBase
{
	GENERATED_BODY()

	FSeaCreatureStats():
		WanderRadius(900.0f),
		SlowRadius(300.0f),
		WanderSpeed(20.0f),
		FleeSpeed(70.0f),
		AttackSpeed(30.0f),
		SeekSpeed(20.0f),
		ReturnSpeed(40.0f),
		Mesh(nullptr) {
	}
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	//float Damage;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	//float HP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	TObjectPtr<class USkeletalMesh> Mesh;






	// 반경
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float WanderRadius; //물고기 배회범위

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float SlowRadius; //물고기 배회범위
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float HomeReturnDist;

	// 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float WanderSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float FleeSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float AttackSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float SeekSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float ReturnSpeed;
};


UCLASS()
class DTDPROJECT_API USeaCreatureData : public UDataAsset
{
	GENERATED_BODY()	
public:
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category ="Stats")
	FSeaCreatureStats* Stats;
};
