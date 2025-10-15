// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractionObject.h"
#include "RandomBox.generated.h"

UCLASS()
class DTDPROJECT_API ARandomBox : public AActor,public IInteractionObject
{
	GENERATED_BODY()
private:
	bool IsOpen{ false };
	UPROPERTY(VisibleAnywhere, Category ="Mesh")
	TObjectPtr<UStaticMeshComponent> BoxFrameMesh;
	UPROPERTY(VisibleAnywhere, Category ="Collision")
	TObjectPtr<class UBoxComponent> BoxCollision;
	
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

};
