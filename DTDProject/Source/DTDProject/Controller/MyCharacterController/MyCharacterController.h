// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyCharacterController.generated.h"

/**
 * 
 */
struct FInputActionValue;
UCLASS()
class DTDPROJECT_API AMyCharacterController : public APlayerController
{
	GENERATED_BODY()
private:
#pragma region Input
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UInputAction> MoveAction; //SWIMMING
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UInputAction> LookAction;
	/*UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UInputAction> EquipAction;*/
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UInputMappingContext> MappingContext;
#pragma endregion
#pragma region Attribute
	TObjectPtr<class UInputAction> AttackAction;
	TObjectPtr<class AMyRobo> ControlledRobo;
	bool isMoveInput{ false };
#pragma endregion
//	UPROPERTY(VisibleAnywhere)
//	TObjectPtr<class UInputAction> InteractionAction;
//	//bool isHit{ true };
public:
	AMyCharacterController();
	void BeginPlay() override;
	void Tick(float DeltaTime) override;
	void SetupInputComponent() override;
	void MoveInput(const FInputActionValue& value);
	void MoveEndInput(const FInputActionValue& value);
	void LookInput(const FInputActionValue& value);
	/*void EquipInput(const FInputActionValue& value);
	void AttackInput(const FInputActionValue& value);*/
	void InteractionInput(const FInputActionValue& value);
//	bool GetIsMoveInput() const { return isMoveInput; }
};
