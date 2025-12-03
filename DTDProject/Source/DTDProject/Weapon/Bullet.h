// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

class USkeletalMeshComponent;
class UProjectileMovementComponent;

UCLASS()
class DTDPROJECT_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	ABullet();

protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	TObjectPtr<USkeletalMeshComponent> BulletMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category="Components")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	float Damage;
};
