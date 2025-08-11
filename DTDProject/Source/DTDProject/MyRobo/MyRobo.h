// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ActorComponent/StateComponent/StateComponent.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
//#include "Interface/AttackTraceNotify/AttackTraceNotify.h"
#include "MyRobo.generated.h"

UCLASS()
class DTDPROJECT_API AMyRobo : public ACharacter
{
	GENERATED_BODY()

private:
#pragma region Component
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> BodyComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USpringArmComponent> SpringArm;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UCameraComponent> Camera;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> WeaponComponent;

#pragma endregion

#pragma region Animation
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAnimMontage> MeleeAttackMontage;
	bool isMeleeAttack{ false };
	UPROPERTY(EditAnywhere, Category = "Montage")
#pragma endregion
	//UPROPERTY(VisibleAnywhere,Category = "State")
	//TObjectPtr<class UStateComponent> StateComponent;
	//UPROPERTY(VisibleAnywhere, Category = "UI")
	//TObjectPtr<class UWidgetComponent> RoboHPBarWidget;
public:
	// Sets default values for this character's properties
	AMyRobo();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode = 0) override;

	void PossessedBy(AController* NewController) override;

	void PlayMontageFullBody(TObjectPtr<UAnimMontage>Montage, FName SectionName = "");

	void PlayMeleeAttackMontage();
};
