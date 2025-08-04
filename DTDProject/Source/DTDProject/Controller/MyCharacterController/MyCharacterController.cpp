// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/MyCharacterController/MyCharacterController.h"
#include "InputAction.h"
#include "inputmappingcontext.h"
#include "enhancedinputsubsystems.h"
#include "EnhancedInputComponent.h"  //InputAction을 관리하는 component
//#include "InputActionValue.h"
#include "MyRobo/MyRobo.h"
//#include "Kismet/KismetSystemLibrary.h"
//#include "Object/Door.h"
//#include "Interface/InteractionObject.h"

AMyCharacterController::AMyCharacterController()
{
	static ConstructorHelpers::FObjectFinder<UInputAction> MoveActionFinder(TEXT("/Script/EnhancedInput.InputAction'/Game/BluePrint/MyCharacter/Input/IA_Move.IA_Move'"));
	if (MoveActionFinder.Succeeded())
	{
		MoveAction = MoveActionFinder.Object;
	}
	/*static ConstructorHelpers::FObjectFinder<UInputAction> EquipActionFinder(TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprints/MyCharacter/Input/IA_Equip_Ch.IA_Equip_Ch'"));
	if (EquipActionFinder.Succeeded())
	{
		EquipAction = EquipActionFinder.Object;
	}*/
	/*static ConstructorHelpers::FObjectFinder<UInputAction> AttackActionFinder(TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprints/MyCharacter/Input/IA_Attack_Ch.IA_Attack_Ch'"));
	if (AttackActionFinder.Succeeded())
	{
		AttackAction = AttackActionFinder.Object;
	}*/
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextFinder(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/BluePrint/MyCharacter/Input/IMC_Character.IMC_Character'"));
	if (InputMappingContextFinder.Succeeded())
	{
		MappingContext = InputMappingContextFinder.Object;
	}

	/*static ConstructorHelpers::FObjectFinder<UInputAction> InteractionActionFinder(TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprints/MyCharacter/Input/IA_Interaction_Ch.IA_Interaction_Ch'"));
	if (InteractionActionFinder.Succeeded())
		InteractionAction = InteractionActionFinder.Object;*/

	PrimaryActorTick.bCanEverTick = true;
}

void AMyCharacterController::BeginPlay()
{
	Super::BeginPlay();
	UEnhancedInputLocalPlayerSubsystem* InputSystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (InputSystem != nullptr)
	{
		InputSystem->AddMappingContext(MappingContext, 0);
	}
	ControlledRobo = Cast<AMyRobo>(GetCharacter());
}

void AMyCharacterController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	/*FHitResult HitResult;
	bool isHit = UKismetSystemLibrary::BoxTraceSingle(this,
		ControlledCharacter->GetActorLocation(),
		ControlledCharacter->GetActorLocation() + ControlledCharacter->GetActorForwardVector() * 500.0f,
		FVector(50.0f, 50.0f, 100.0f),
		FRotator::ZeroRotator,
		UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel4),
		false,
		{ ControlledCharacter },
		EDrawDebugTrace::ForOneFrame,
		HitResult,
		true
	);
	if (isHit)
		ControlledCharacter->SetInteractionObject(Cast<IInteractionObject>(HitResult.GetActor()));
	else
		ControlledCharacter->SetInteractionObject(nullptr);*/
}

void AMyCharacterController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UEnhancedInputComponent* input = Cast<UEnhancedInputComponent>(InputComponent);
	if (input != nullptr)
	{
		input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyCharacterController::MoveInput);
		input->BindAction(MoveAction, ETriggerEvent::Completed, this, &AMyCharacterController::MoveEndInput);
		/*input->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyCharacterController::LookInput);
		input->BindAction(EquipAction, ETriggerEvent::Started, this, &AMyCharacterController::EquipInput);
		input->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AMyCharacterController::AttackInput);*/
		//input->BindAction(InteractionAction, ETriggerEvent::Started, this, &AMyCharacterController::InteractionInput);
	}
}

void AMyCharacterController::MoveInput(const FInputActionValue& value)
{
	isMoveInput = true;
	FVector2D MoveValue = value.Get<FVector2D>();
	FVector Forward = GetTransformComponent()->GetForwardVector();
	Forward.Z = 0.0;
	Forward.Normalize();
	ControlledRobo->AddMovementInput(Forward, MoveValue.X);
	ControlledRobo->AddMovementInput(GetTransformComponent()->GetRightVector(), MoveValue.Y);
}

void AMyCharacterController::MoveEndInput(const FInputActionValue& value)
{
	isMoveInput = false;
}

//void AMyCharacterController::LookInput(const FInputActionValue& value)
//{
//	/*FVector2D MoveValue = value.Get<FVector2D>();
//	AddYawInput(MoveValue.X);
//	AddPitchInput(MoveValue.Y);*/
//}
// 
//void AMyCharacterController::EquipInput(const FInputActionValue& value)
//{
//	//ControlledCharacter->PlayEquipWeaponMontage();
//}
//
//void AMyCharacterController::AttackInput(const FInputActionValue& value)
//{
//	//ControlledCharacter->PlayAttackMontage();
//}

void AMyCharacterController::InteractionInput(const FInputActionValue& value)
{
	/*if (ControlledCharacter)
		ControlledCharacter->InteractionAction();*/
}