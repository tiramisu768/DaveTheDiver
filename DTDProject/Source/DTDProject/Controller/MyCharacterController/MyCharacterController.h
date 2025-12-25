// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UI/MainUI.h"
#include "MyCharacterController.generated.h"

struct FInputActionValue;

// 게임 상태 enum도 MainUI나 GameMode로 이전하는 것이 좋습니다.
// 일단 여기서는 유지하되, UI와 직접 관련된 상태는 제거합니다.
UENUM(BlueprintType)
enum class EPlayerGameState :uint8
{
	Diving,

};

UCLASS()
class DTDPROJECT_API AMyCharacterController : public APlayerController
{
	GENERATED_BODY()

public:
	AMyCharacterController();
	UMainUI* GetMainUI() const { return Cast<UMainUI>(MainWidgetInstance); }
	bool GetIsAttacking() const { return IsAttacking; }
	void SetIsAttacking(bool bNewState) { IsAttacking = bNewState; }
	void BeginPlay() override;
	void Tick(float DeltaTime) override;
	void SetupInputComponent() override;
	void MoveInput(const FInputActionValue& value);
	void MoveEndInput(const FInputActionValue& value);
	void LookInput(const FInputActionValue& value);
	void DashInput(const FInputActionValue& value);
	void OnFireTriggered(const FInputActionValue& value);
	void OnFireStopped(const FInputActionValue& value);
	void StartAiming(const FInputActionValue& value);
	void StopAiming(const FInputActionValue& value);
	void SwitchWeaponInput(const FInputActionValue& value);
	void UseToolInput(const FInputActionValue& value);
	void SwitchToolInput(const FInputActionValue& value);
	/*void EquipInput(const FInputActionValue& value);*/
	void InteractionStarted(const FInputActionValue& value);
	void InteractionCompleted(const FInputActionValue& value);
	void OnNavigateUp();
	void OnNavigateDown();
	void OnSelectUIButton();
	bool FireStartPosition(FVector& WorldPosition, FVector& WorldDirection);
	//	bool GetIsMoveInput() const { return isMoveInput; }
	void EndMyGame(bool IsSuccess);

private:
	EPlayerGameState CurrentGameState;
	TSubclassOf<class UMainUI> MainWidgetClass;

	UPROPERTY()
	class UMainUI* MainWidgetInstance;

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
	TObjectPtr<class UInputAction> NavigateUpAction;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UInputAction> NavigateDownAction;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UInputAction> SelectUIButtonAction;
	UPROPERTY(VisibleAnywhere,Category = "Input")
	TObjectPtr<class UInputMappingContext> GameMappingContext;
	UPROPERTY(VisibleAnywhere,Category = "Input")
	TObjectPtr<class UInputMappingContext> UIMappingContext;
	float SpacePressedTime =0.f;
	UPROPERTY(EditAnywhere, Category = "Input")
	float HoldThreshold = 0.5f;
#pragma endregion
#pragma region Attribute
	TObjectPtr<class AMyRobo> ControlledRobo;
	bool isMoveInput{ false };
#pragma endregion
	//Aim 상태 관리
	bool IsAttacking;
	FVector AimDirection;
	FVector2D AimScreenPos;

	bool IsAiming;

	//무기 선택 상태 관리
	int32 CurrentWeaponIndex = 0;
	const int32 WeaponCount = 2;

	FVector2D PrevMousePosition{-1,-1};
	virtual void OnPossess(APawn* aPawn) override;
//	//bool isHit{ true };
};
