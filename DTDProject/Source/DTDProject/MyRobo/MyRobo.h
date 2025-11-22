// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"
#include "Interface/AttackTraceNotify/AttackTraceNotify.h"
#include "Weapon/Weapon.h"
#include "Weapon/WeaponData.h"
#include "MyRobo.generated.h"

class UAIPerceptionStimuliSourceComponent;
class AMyCharacterController;
class UMainUI;
class ARandomBox;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWeaponSlotUpdated, EWeaponSlot, Slot, AWeapon*, NewWeapon);

UCLASS()
class DTDPROJECT_API AMyRobo : public ACharacter, public IAttackTraceNotify, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

private:
	FGenericTeamId TeamId;

#pragma region Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AI",meta = (AllowPrivateAccess="true"))
	TObjectPtr<UAIPerceptionStimuliSourceComponent> StimuliSourceComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> BodyComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USpringArmComponent> SpringArm;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UCameraComponent> Camera;
	UPROPERTY(VisibleAnywhere, Category = "State")
	TObjectPtr<class URoboComponent> RoboComponent;
	UPROPERTY(VisibleAnywhere, Category = "Buoyancy")
	TObjectPtr<class UBuoyancyComponent> BuoyancyComponent;
	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TObjectPtr<class UInventoryComponent> InventoryComponent;

#pragma endregion

#pragma region Animation
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAnimMontage> MeleeAttackMontage;
	bool isMeleeAttack{ false };
	bool isEquip{ false };
#pragma endregion

#pragma region Water
	float DepthBelowSurface = 0.f; // +면 수면 아래, -면 수면 위
#pragma endregion

#pragma region Weapon
	UPROPERTY(EditAnywhere, Category = "Weapon")
	float TargetWeaponSize = 30.0f;

	UPROPERTY(VisibleAnywhere, Category="Weapon")
	TObjectPtr<AWeapon> MeleeWeapon;

	UPROPERTY(VisibleAnywhere, Category="Weapon")
	TObjectPtr<AWeapon> HarpoonWeapon;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	TObjectPtr<AWeapon> GunWeapon;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	TObjectPtr<AWeapon> ActiveRangedWeapon;

	UPROPERTY()
	TObjectPtr<AWeapon> AcquirableWeapon;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	TSubclassOf<AWeapon> DefaultMeleeWeaponClass;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<AWeapon> DefaultHarpoonWeaponClass;

	UPROPERTY(EditDefaultsOnly, Category ="Weapon")
	TSubclassOf<AWeapon> DefaultGunWeaponClass;

	void PickupAcquirableWeapon();

#pragma endregion

#pragma region Interaction
	class IInteractionObject* InteractionObject;
	UPROPERTY()
	TScriptInterface<IInteractionObject> CurrentInteractable;
	UPROPERTY(VisibleAnywhere, Category = "Interaction")
	TObjectPtr<class UWidgetComponent> LongPressWidget;
	UPROPERTY(VisibleAnywhere, Category = "Interaction")
	TObjectPtr<class UWidgetComponent> PickupWidget;
	TSubclassOf<class UUserWidget> LongPressWidgetClass;
	TSubclassOf<class UUserWidget> PickupWidgetClass;
	FTimerHandle HoldTimerHandle;
	float HoldDuration = 2.0f;
	bool IsHolding = false;
	float HoldElapsed = 0.0f;
	bool bIsCameraFixed = false;
#pragma endregion

protected:
	UPROPERTY()
	TObjectPtr<AMyCharacterController> MainController;
	virtual void BeginPlay() override;

public:	
	AMyRobo();

	UPROPERTY(BlueprintAssignable, Category = "Weapon")
	FOnWeaponSlotUpdated OnWeaponSlotUpdated;

	virtual FGenericTeamId GetGenericTeamId() const override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode = 0) override;

	void PossessedBy(AController* NewController) override;

	void setupMainUIReference(UMainUI* InMainUI);

	void PlayMontageFullBody(TObjectPtr<UAnimMontage>Montage, FName SectionName = "");

	void PlayMeleeAttackMontage();

	void PlayRangedAttackMontage();

	void PerformAttack();

	float GetDepthBelowSurface() const;

	void AttackTrace() override;

	void SwitchActiveRangedWeapon();

	void SetCurrentInteractable(TScriptInterface<IInteractionObject> NewInteractable){CurrentInteractable = NewInteractable;}
	
	void ShowLongPressWidget(bool bShow, AActor* TargetActor);

	void ShowPickupWidget(bool bShow, AActor* TargetActor);

	void StartSpaceHold();

	void StopSpaceHold();

	void SetAcquirableWeapon(AWeapon* Weapon);

	void HandleShortPress();

	void UpdateInteractionProgress(float Percent);

	void FocusOnInteractionTarget(IInteractionObject* Target);

	UInventoryComponent* GetInventoryComponent() const { return InventoryComponent; }
};
