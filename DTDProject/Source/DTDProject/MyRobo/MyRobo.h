// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"
#include "Interface/AttackTraceNotify/AttackTraceNotify.h"
#include "Weapon/Weapon.h"
#include "Weapon/WeaponData.h"
#include "Interface/InteractionObject.h"
#include "ActorComponent/InventoryComponent.h"
#include "MyRobo.generated.h"

class UAIPerceptionStimuliSourceComponent;
class AMyCharacterController;
class UMainUI;
class ARandomBox;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWeaponSlotUpdated, EWeaponSlot, Slot, AWeapon*, NewWeapon);

UENUM(BlueprintType)
enum class EWeaponState :uint8
{
	Unarmed, //비무장
	MeleeAttaching,
	RangedAttaching
};

UCLASS()
class DTDPROJECT_API AMyRobo : public ACharacter, public IAttackTraceNotify, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

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

	void PerformAttack(const FVector& AimDirection = FVector::ZeroVector);

	void SwitchActiveRangedWeapon();

	void FireRangedWeapon(const FVector& AimDirection);

	void SetCurrentInteractable(TScriptInterface<IInteractionObject> NewInteractable) { CurrentInteractable = NewInteractable; }

	void ShowLongPressWidget(bool bShow, AActor* TargetActor);

	void ShowPickupWidget(bool bShow, AActor* TargetActor);

	void StartSpaceHold();

	void StopSpaceHold();
	
	void SetAcquirableWeapon(AWeapon* Weapon);

	void FocusOnInteractionTarget(IInteractionObject* Target);

	UInventoryComponent* GetInventoryComponent() const { return InventoryComponent; }

	void BroadcastCurrentWeaponStates();

	void AttackTrace() override;

protected:
	virtual void BeginPlay() override;

#pragma region Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> BodyComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> SpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> Camera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class URoboComponent> RoboComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBuoyancyComponent> BuoyancyComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInventoryComponent> InventoryComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAIPerceptionStimuliSourceComponent> StimuliSourceComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UWidgetComponent> LongPressWidget;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UWidgetComponent> PickupWidget;
#pragma endregion

#pragma region Weapon
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName MeleeWeaponSocketName = TEXT("MeleeWeaponSocket");

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName HarpoonWeaponSocketName = TEXT("HarpoonWeaponSocket");

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName GunWeaponSocketName = TEXT("GunWeaponSocket");

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Weapon")
	float TargetWeaponSize = 30.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<AWeapon> DefaultMeleeWeaponClass;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<AWeapon> DefaultHarpoonWeaponClass;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<AWeapon> DefaultGunWeaponClass;
#pragma endregion

#pragma region Internal State
	UPROPERTY()
	TObjectPtr<AMyCharacterController> MainController;

	EWeaponState CurrentWeaponState = EWeaponState::Unarmed;
	FTimerHandle HolsterTimerHandle;
#pragma endregion

#pragma region Internal Functions
	void PlayMontageFullBody(TObjectPtr<UAnimMontage>Montage, FOnMontageEnded& EndDelegate, FName SectionName = "");

	void PlayMeleeAttackMontage();

	void PlayRangedAttackMontage();

	void HandleShortPress();

	void UpdateInteractionProgress(float Percent);

	void PickupAcquirableWeapon();

	void UpdateWeaponAttachments();

	void StartHolsterTimer();

	void HolsterWeapons();

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	float GetDepthBelowSurface() const;
#pragma endregion

	private:
		FGenericTeamId TeamId;

#pragma region Weapon Instances
	UPROPERTY()
	TObjectPtr<AWeapon> MeleeWeapon;

	UPROPERTY()
	TObjectPtr<AWeapon> HarpoonWeapon;

	UPROPERTY()
	TObjectPtr<AWeapon> GunWeapon;

	UPROPERTY()
	TObjectPtr<AWeapon> ActiveRangedWeapon;

	UPROPERTY()
	TObjectPtr<AWeapon> AcquirableWeapon;
#pragma endregion

#pragma region Interaction
	UPROPERTY()
	TScriptInterface<IInteractionObject> CurrentInteractable;

	TSubclassOf<class UUserWidget> LongPressWidgetClass;
	TSubclassOf<class UUserWidget> PickupWidgetClass;

	float HoldDuration = 2.0f;
	bool IsHolding = false;
	float HoldElapsed = 0.0f;
	bool bIsCameraFixed = false;
#pragma endregion

#pragma region Animation
	UPROPERTY()
	TObjectPtr<UAnimMontage> MeleeAttackMontage;
	UPROPERTY()
	TObjectPtr<UAnimMontage> Melee2AttackMontage;
	UPROPERTY()
	TObjectPtr<UAnimMontage> Melee3AttackMontage;
	UPROPERTY()
	TObjectPtr<UAnimMontage> RangedAttackMontage;
#pragma endregion

#pragma region Water
	float DepthBelowSurface = 0.f; // +면 수면 아래, -면 수면 위
#pragma endregion
};
