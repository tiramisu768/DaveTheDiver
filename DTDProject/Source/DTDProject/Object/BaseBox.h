// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractionObject.h"
#include "BaseBox.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class AMyRobo;

UCLASS()
class DTDPROJECT_API ABaseBox : public AActor, public IInteractionObject
{
	GENERATED_BODY()

public:
	ABaseBox();

	virtual void Interact(AMyRobo* InteractingRobo) override;
	virtual void ShowInteractionWidget(bool bShow) override;
	virtual void StartFocus() override;
	virtual void EndFocus() override;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void OnBoxOpened() PURE_VIRTUAL(ABaseBox::OnBoxOpened, );

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> BoxFrameMesh;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UBoxComponent> BoxCollision;

	UPROPERTY(BlueprintReadOnly, Category = "State")
	bool IsOpen{ false };

	UPROPERTY(BlueprintReadOnly, Category = "State")
	bool IsOpening{ false };

	UPROPERTY()
	TObjectPtr<AMyRobo> CurrentInteractingRobo;

private:
	void UpdateOpenAnimation(float DeltaTime);

	UPROPERTY()
	TArray<TObjectPtr<class UMaterialInstanceDynamic>> DynMats;

	UPROPERTY(EditAnywhere, Category = "BaseBox|Visual")
	float Brightness = 1.0f;

	UPROPERTY(EditAnywhere, Category = "BaseBox|Visual")
	float OpenSpeed = 2.0f;

	bool IsRoboOverlap{ false };

};
