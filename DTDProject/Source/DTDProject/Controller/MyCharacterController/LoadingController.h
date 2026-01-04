// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LoadingController.generated.h"

class UUserWidget;

UCLASS()
class DTDPROJECT_API ALoadingController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALoadingController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category="Loading")
	TSubclassOf<UUserWidget> LoadingWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> LoadingWidgetInstance;

	FName NextMapToLoad;

	UFUNCTION()
	void LoadNextMap();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
