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
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UInputAction> DashAction;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UInputAction> MeleeAttackAction;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UInputAction> RangedAttackAction;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UInputAction> SwitchWeaponAction;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UInputAction> UseToolAction;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UInputAction> SwitchToolAction;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UInputAction> InteractionAction;
	/*UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UInputAction> EquipAction;*/
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UInputMappingContext> MappingContext;
	float SpacePressedTime =0.f;
	UPROPERTY(EditAnywhere, Category = "Input")
	float HoldThreshold = 0.5f;
#pragma endregion
#pragma region Attribute
	TObjectPtr<class AMyRobo> ControlledRobo;
	bool isMoveInput{ false };
#pragma endregion
	//Aim 상태 관리
	bool IsAiming;
	FVector AimDirection;
	FVector2D AimScreenPos = FVector2D(960.f, 540.f);

	//무기 선택 상태 관리
	int32 CurrentWeaponIndex = 0;
	const int32 WeaponCount = 2;

//	//bool isHit{ true };
public:
	AMyCharacterController();
	void BeginPlay() override;
	void Tick(float DeltaTime) override;
	void SetupInputComponent() override;
	void MoveInput(const FInputActionValue& value);
	void MoveEndInput(const FInputActionValue& value);
	void LookInput(const FInputActionValue& value);
	void DashInput(const FInputActionValue& value);
	void MeleeAttackInput(const FInputActionValue& value);
	void MoveAimPoint(const FVector2D& MoveValue);
	void StartAiming(const FInputActionValue& value);
	void StopAiming(const FInputActionValue& value);
	void UpdateAimDirection(const FInputActionValue& value);
	void SwitchWeaponInput(const FInputActionValue& value);
	void UseToolInput(const FInputActionValue& value);
	void SwitchToolInput(const FInputActionValue& value);
	/*void EquipInput(const FInputActionValue& value);*/
	void InteractionStarted(const FInputActionValue& value);
	void InteractionCompleted(const FInputActionValue& value);
//	bool GetIsMoveInput() const { return isMoveInput; }
};
