// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractionObject.h"
#include "Weapon/Weapon.h"
#include "RandomBox.generated.h"

UCLASS()
class DTDPROJECT_API ARandomBox : public AActor,public IInteractionObject
{
	GENERATED_BODY()
private:
	bool IsOpen{ false };
	bool IsRoboOverlap{ false };
	bool IsOpening{ false }; //오픈 애님 진행 여부
	UPROPERTY(VisibleAnywhere, Category ="Mesh")
	TObjectPtr<UStaticMeshComponent> BoxFrameMesh;
	UPROPERTY(VisibleAnywhere, Category ="Collision")
	TObjectPtr<class UBoxComponent> BoxCollision;
	UMaterialInstanceDynamic* DynMat;
	float Brightness = 1.0f;
	UPROPERTY(EditDefaultsOnly,Category = "RandomBox")
	TSubclassOf<AWeapon> TestWeaponClass;
	
public:	
	// Sets default values for this actor's properties
	ARandomBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void Interact() override;
	UFUNCTION()
	void RandomBoxOnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void RandomBoxOnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	void BeginOpen(); //로봇이 홀드완료 시 호출
	void OpenRandomBox(float DeltaTime); //밝기 애니메이션 실행
	void SpawnWeaponOnBox();
	bool GetIsOpen() const { return IsOpen; }
	bool GetIsOpening() const { return IsOpening; }

};
