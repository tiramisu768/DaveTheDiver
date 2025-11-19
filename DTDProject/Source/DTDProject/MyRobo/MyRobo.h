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


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponChanged, AWeapon*, NewWeapon);

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
	UPROPERTY(VisibleAnywhere, Category="Weapon")
	TObjectPtr<AWeapon> CurrentWeapon;

	UPROPERTY(VisibleAnywhere, Category="Weapon")
	TArray<FWeaponTypeInventory> WeaponInventory;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	TSubclassOf<AWeapon> DefaultMeleeWeaponClass;

	UPROPERTY(EditDefaultsOnly, Category ="Weapon")
	TSubclassOf<AWeapon> DefaultRangedWeaponClass;

	void AddWeaponToInventory(AWeapon* WeaponToAdd);
	void EquipWeapon(AWeapon* WeaponToEquip);
	void SwitchNextWeapon(EWeaponType TypeToSwitch);
	AWeapon* FindNearbyWeapon();

	FWeaponTypeInventory* GetInventoryForType(EWeaponType WeaponType);

#pragma endregion

#pragma region Interaction
	class IInteractionObject* InteractionObject;
	UPROPERTY()
	TScriptInterface<IInteractionObject> CurrentInteractable;
	UPROPERTY(VisibleAnywhere, Category = "Interaction")
	TObjectPtr<class UWidgetComponent> InteractionWidget;
	TSubclassOf<class UUserWidget> InteractionWidgetClass;
	FTimerHandle HoldTimerHandle;
	float HoldDuration = 2.0f;
	bool IsHolding = false;
	float HoldElapsed = 0.0f;
#pragma endregion

protected:
	UPROPERTY()
	TObjectPtr<AMyCharacterController> MainController;
	virtual void BeginPlay() override;

public:	
	AMyRobo();

	UPROPERTY(BlueprintAssignable,Category="Weapon")
	FOnWeaponChanged OnWeaponChanged;

	virtual FGenericTeamId GetGenericTeamId() const override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode = 0) override;

	void PossessedBy(AController* NewController) override;

	void setupMainUIReference(UMainUI* InMainUI);

	void PlayMontageFullBody(TObjectPtr<UAnimMontage>Montage, FName SectionName = "");

	void PlayMeleeAttackMontage();

	void AttackSeaCreature();

	float GetDepthBelowSurface() const;

	void AttackTrace() override;

	void SetCurrentInteractable(TScriptInterface<IInteractionObject> NewInteractable){CurrentInteractable = NewInteractable;}
	
	void ShowInteractionWidget(bool bShow);

	void StartSpaceHold();

	void StopSpaceHold();

	void HandleShortPress();

	void UpdateInteractionProgress(float Percent);

	void FocusOnInteractionTarget(IInteractionObject* Target);

	void FireCurrentWeaponAt(const FVector& SpawnLocation, const FVector& AimDirection);

	UInventoryComponent* GetInventoryComponent() const { return InventoryComponent; }
};
