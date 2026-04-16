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
class ASeaCreature;
class UMainUI;
class ALootSpawnerBox;
class AInstantRewardBox;
class APickupItem;

DECLARE_MULTICAST_DELEGATE(FOnSurfaced);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnWeaponSlotUpdated, EWeaponSlot, AWeapon*);

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

	FOnSurfaced OnSurfaced;
	FOnWeaponSlotUpdated OnWeaponSlotUpdated;

	virtual FGenericTeamId GetGenericTeamId() const override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode = 0) override;

	void SetupMainUIReference(UMainUI* InMainUI);

	void BeginRangedAim();

	void EndRangedAim();

	void StartFiring(const FVector& FireDirection);

	void StopFiring();

	void PerformMeleeAttack();

	void PerformAttack(const FVector2D& ScreenPosition);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void FireProjectile();

	void SwitchActiveRangedWeapon();

	UFUNCTION(BlueprintCallable,Category="Weapon")
	AWeapon* GetActiveWeapon() const;

	AWeapon* GetGunWeapon() const { return GunWeapon; }

	void SetCurrentInteractable(TScriptInterface<IInteractionObject> NewInteractable) { CurrentInteractable = NewInteractable; }

	void ShowLongPressWidget(bool bShow, AActor* TargetActor);

	void ShowPickupWidget(bool bShow, AActor* TargetActor);

	void StartSpaceHold();

	void StopSpaceHold();
	
	void SetAcquirableActor(AActor* Actor);

	void FocusOnInteractionTarget(IInteractionObject* Target);

	UInventoryComponent* GetInventoryComponent() const { return InventoryComponent; }

	void BroadcastCurrentWeaponStates();

	void OnToolReplaceConfirmed(int32 SlotIndexToDiscard);

	void AttackTrace() override;

	void CollectSeaCreature(ASeaCreature* FishToCollect);

	void HitBy(AActor* DamageCauser, const FHitResult& HitResult);

	void DieRobo();

	bool DeprojectAimToWorld(FVector& WorldPosition, FVector& WorldDirection);
	
	UFUNCTION(Client,Reliable)
	void Client_DrawAimWidget();

	UFUNCTION(Server,Reliable)
	void Server_CanAttack();

	UFUNCTION(Server, Reliable)
	void Server_MeleeAttack();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayMeleeAttackMontage();

	UFUNCTION(Server, Reliable)
	void Server_SetAim(bool bIsAim);

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	//프라이빗으로 옮기기
	UPROPERTY(Replicated)
	bool IsAttacking;
	UPROPERTY(Replicated)
	bool IsAiming;

protected:
	virtual void PostInitializeComponents() override;

	void PossessedBy(AController* NewController) override;

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
	FName RangedWeaponSocketName = TEXT("RangedWeaponSocket");

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName XWeaponSocketName = TEXT("XWeaponSocket");

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<AWeapon> DefaultMeleeWeaponClass;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<AWeapon> DefaultHarpoonWeaponClass;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<AWeapon> DefaultGunWeaponClass;
#pragma endregion

#pragma region Internal State
	UPROPERTY()
	TWeakObjectPtr<AMyCharacterController> MainController;

	EWeaponState CurrentWeaponState = EWeaponState::Unarmed;
	FTimerHandle HolsterTimerHandle;
#pragma endregion

#pragma region Internal Functions
	void PlayMontageFullBody(TObjectPtr<UAnimMontage>Montage, FOnMontageEnded& EndDelegate, FName SectionName = "", float PlayRate = 1.0f);

	void PlayMeleeAttackMontage();

	void PlayRangedAimMontage();

	void PlayRangedStopAimMontage();

	void PlayRangedAttackMontage();

	void StopAnimMontage(float BlendOutTime = 0.25f);

	void HandleShortPress();

	void UpdateInteractionProgress(float Percent);

	void PickupAcquirableWeapon();

	void PickupAcquirableTool();

	void UpdateWeaponAttachments();

	void StartHolsterTimer();

	void HolsterWeapons();

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void OnStopAimMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	float GetDepthBelowSurface() const;
#pragma endregion

private:
	FGenericTeamId TeamId;
	bool WasOnSurface;

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
	TWeakObjectPtr<class AActor> AcquirableActor;

	UPROPERTY()
	TWeakObjectPtr<APickupItem> PendingToolToPickup;

	UPROPERTY(EditDefaultsOnly, Category = "Socket")
	FName ToolSocketName;

	FVector2D RangedTargetScreenPosition;

	void EquipWeaponToSlot(AWeapon* WeaponToPickup, EWeaponSlot Slot);
#pragma endregion

#pragma region Interaction
	UPROPERTY()
	TScriptInterface<IInteractionObject> CurrentInteractable;

	TSubclassOf<class UUserWidget> LongPressWidgetClass;
	TSubclassOf<class UUserWidget> PickupWidgetClass;

	UPROPERTY()
	TObjectPtr<UAudioComponent> InteractionSoundComponent;

	float HoldDuration = 2.0f;
	bool IsHolding = false;
	float HoldElapsed = 0.0f;
	bool bIsCameraFixed = false;
	bool bIsFiring = false;
#pragma endregion

#pragma region Animation
	UPROPERTY()
	TObjectPtr<UAnimMontage> HitReactionMontage;
#pragma endregion

#pragma region Water
	bool bWasSwimmingAndMoving = false;
	float DepthBelowSurface = 0.f; // +면 수면 아래, -면 수면 위
#pragma endregion
};
