// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractionObject.h"
#include "RandomBox.generated.h"

class UBoxComponent;
class AWeapon;
class UDataTable;

UCLASS()
class DTDPROJECT_API ARandomBox : public AActor,public IInteractionObject
{
	GENERATED_BODY()

public:
	ARandomBox();

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void Interact(AMyRobo* InteractingRobo) override;
	virtual void ShowInteractionWidget(bool bShow) override;
	virtual void StartFocus() override;
	virtual void EndFocus() override;

	UFUNCTION()
	void RandomBoxOnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void RandomBoxOnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	bool IsOpen{ false };
	bool IsRoboOverlap{ false };
	bool IsOpening{ false }; //오픈 애님 진행 여부

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> BoxFrameMesh;

	UPROPERTY(EditAnywhere, Category ="Box Properties")
	TObjectPtr<class UBoxComponent> BoxCollision;

	UPROPERTY(EditAnywhere,Category = "Box Properties")
	UMaterialInstanceDynamic* DynMat;

	float Brightness = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Box Properties", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDataTable> WeaponDataTable;

	UPROPERTY()
	TObjectPtr<AWeapon> SpawnedWeapon;

	UPROPERTY()
	TObjectPtr<AMyRobo> CurrentInteractingRobo;

	void UpdateOpenAnimation(float DeltaTime); //밝기 애니메이션 실행
	void SpawnWeapon();

};
