// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamagePopup.generated.h"

UCLASS()
class DTDPROJECT_API ADamagePopup : public AActor
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, Category = "Hide", meta = (AllowPrivateAccess = "true"))
	float HideInterval=3.0f;
	FTimerHandle HideTimerHandle;
public:	
	// Sets default values for this actor's properties
	ADamagePopup();
	UPROPERTY(VisibleAnywhere, Category = "UI")
	TObjectPtr<class UWidgetComponent> SimpleDamageWidget;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void HideDamagePopup();
	void HidePopup();
	void GetDamage();
};
