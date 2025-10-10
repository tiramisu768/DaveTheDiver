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
#include "HUD/MyHUD.h"

AMyCharacterController::AMyCharacterController()
{
	static ConstructorHelpers::FObjectFinder<UInputAction> MoveActionFinder(TEXT("/Script/EnhancedInput.InputAction'/Game/BluePrint/MyRobo/Input/IA_Move.IA_Move'"));
	if (MoveActionFinder.Succeeded())
	{
		MoveAction = MoveActionFinder.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> LookActionFinder(TEXT("/Script/EnhancedInput.InputAction'/Game/BluePrint/MyRobo/Input/IA_Rotation.IA_Rotation'"));
	if (LookActionFinder.Succeeded())
	{
		LookAction = LookActionFinder.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> DashActionFinder(TEXT("/Script/EnhancedInput.InputAction'/Game/BluePrint/MyRobo/Input/IA_Dash.IA_Dash'"));
	if (DashActionFinder.Succeeded())
	{
		DashAction = DashActionFinder.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> MeleeAttackActionFinder(TEXT("/Script/EnhancedInput.InputAction'/Game/BluePrint/MyRobo/Input/IA_MeleeAttack.IA_MeleeAttack'"));
	if (MeleeAttackActionFinder.Succeeded())
	{
		MeleeAttackAction = MeleeAttackActionFinder.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> RangedAttackActionFinder(TEXT("/Script/EnhancedInput.InputAction'/Game/BluePrint/MyRobo/Input/IA_RangedAttack.IA_RangedAttack'"));
	if (RangedAttackActionFinder.Succeeded())
	{
		RangedAttackAction = RangedAttackActionFinder.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> SwitchWeaponActionFinder(TEXT("/Script/EnhancedInput.InputAction'/Game/BluePrint/MyRobo/Input/IA_SwitchWeapon.IA_SwitchWeapon'"));
	if (SwitchWeaponActionFinder.Succeeded())
	{
		SwitchWeaponAction = SwitchWeaponActionFinder.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> UseToolActionFinder(TEXT("/Script/EnhancedInput.InputAction'/Game/BluePrint/MyRobo/Input/IA_UseTool.IA_UseTool'"));
	if (UseToolActionFinder.Succeeded())
	{
		UseToolAction = UseToolActionFinder.Object;
	}	
	static ConstructorHelpers::FObjectFinder<UInputAction> SwitchToolActionFinder(TEXT("/Script/EnhancedInput.InputAction'/Game/BluePrint/MyRobo/Input/IA_SwitchTool.IA_SwitchTool'"));
	if (SwitchToolActionFinder.Succeeded())
	{
		SwitchToolAction = SwitchToolActionFinder.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> SpacePressActionFinder(TEXT("/Script/EnhancedInput.InputAction'/Game/BluePrint/MyRobo/Input/IA_SpacePress.IA_SpacePress'"));
	if (SpacePressActionFinder.Succeeded())
	{
		SpacePressAction = SpacePressActionFinder.Object;
	}
	/*static ConstructorHelpers::FObjectFinder<UInputAction> EquipActionFinder(TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprints/MyRobo/Input/IA_Equip_Ch.IA_Equip_Ch'"));
	if (EquipActionFinder.Succeeded())
	{
		EquipAction = EquipActionFinder.Object;
	}*/

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextFinder(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/BluePrint/MyRobo/Input/IMC_Character.IMC_Character'"));
	if (InputMappingContextFinder.Succeeded())
	{
		MappingContext = InputMappingContextFinder.Object;
	}

	/*static ConstructorHelpers::FObjectFinder<UInputAction> InteractionActionFinder(TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprints/MyRobo/Input/IA_Interaction_Ch.IA_Interaction_Ch'"));
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

	//if (HUDClass)
	//{
	//	AMyHUD* MyHUD = GetWorld()->SpawnActor<AMyHUD>(HUDClass);
	//	if (MyHUD)
	//	{
	//		SetHUD(MyHUD);
	//	}
	//}
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

void AMyCharacterController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	AMyHUD* controlledHUD = Cast<AMyHUD>(GetHUD());
	if (controlledHUD)
	{
		float MouseX, MouseY;
		GetMousePosition(MouseX, MouseY);

		FVector2D MousePos(MouseX, MouseY);
		FVector2D Dir = MousePos - controlledHUD->GetRoboAimUI()->GetArcCenter();
		float Angle = FMath::Atan2(Dir.Y, Dir.X);

		Angle = FMath::Clamp(Angle, -60.f * PI / 180.f, 60.f * PI / 180.f);

		controlledHUD->SetAimAngle(Angle);
	}
}

void AMyCharacterController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UEnhancedInputComponent* input = Cast<UEnhancedInputComponent>(InputComponent);
	if (input != nullptr)
	{
		input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyCharacterController::MoveInput);
		input->BindAction(MoveAction, ETriggerEvent::Completed, this, &AMyCharacterController::MoveEndInput);
		input->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyCharacterController::LookInput);
		input->BindAction(DashAction, ETriggerEvent::Started, this, &AMyCharacterController::DashInput);
		input->BindAction(MeleeAttackAction, ETriggerEvent::Started, this, &AMyCharacterController::MeleeAttackInput);
		input->BindAction(RangedAttackAction, ETriggerEvent::Started, this, &AMyCharacterController::StartAiming); //우클릭 시작 시
		input->BindAction(RangedAttackAction, ETriggerEvent::Completed, this, &AMyCharacterController::StopAiming); //우클릭 끝 시
		input->BindAction(RangedAttackAction, ETriggerEvent::Triggered, this, &AMyCharacterController::UpdateAimDirection); //마우스 이동 시
		input->BindAction(SwitchWeaponAction, ETriggerEvent::Started, this, &AMyCharacterController::SwitchWeaponInput);
		input->BindAction(UseToolAction, ETriggerEvent::Started, this, &AMyCharacterController::UseToolInput);
		input->BindAction(SwitchToolAction, ETriggerEvent::Started, this, &AMyCharacterController::SwitchToolInput);
		input->BindAction(SpacePressAction, ETriggerEvent::Started, this, &AMyCharacterController::OnSpaceStarted);
		input->BindAction(SpacePressAction, ETriggerEvent::Completed, this, &AMyCharacterController::OnSpaceCompleted);
		/*input->BindAction(EquipAction, ETriggerEvent::Started, this, &AMyCharacterController::EquipInput);*/
		//input->BindAction(InteractionAction, ETriggerEvent::Started, this, &AMyCharacterController::InteractionInput);
	}
}

void AMyCharacterController::MoveInput(const FInputActionValue& value)
{
	isMoveInput = true;
	FVector2D MoveValue = value.Get<FVector2D>();
	//FVector Forward = GetTransformComponent()->GetForwardVector();
	//Forward.Z = 0.0;
	//Forward.Normalize();
	ControlledRobo->AddMovementInput(GetTransformComponent()->GetForwardVector(), MoveValue.X);
	ControlledRobo->AddMovementInput(GetTransformComponent()->GetRightVector(), MoveValue.Y);
}

void AMyCharacterController::MoveEndInput(const FInputActionValue& value)
{
	isMoveInput = false;
}

void AMyCharacterController::LookInput(const FInputActionValue& value)
{
	FVector2D MoveValue = value.Get<FVector2D>();
	AddYawInput(MoveValue.X);
	AddPitchInput(MoveValue.Y);
}

void AMyCharacterController::DashInput(const FInputActionValue& value)
{
	ControlledRobo->LaunchCharacter(ControlledRobo->GetActorForwardVector() * 800, true, true);
	//좌우뒤 대쉬도 추가하려면 방향키에 대한 인풋값을 따로 저장해서 인풋키가 뭔지를 인지하고 거기에다가 벨로시티를 곱해주도록 수정해야한다
}

void AMyCharacterController::MeleeAttackInput(const FInputActionValue& value)
{
	ControlledRobo->PlayMeleeAttackMontage();
}

void AMyCharacterController::StartAiming(const FInputActionValue& value)
{
	IsAiming = true;
	AMyHUD* ControlledHUD = Cast<AMyHUD>(GetHUD());
	if (ControlledHUD && ControlledHUD->GetRoboAimUI())
	{
		ControlledHUD->GetRoboAimUI()->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMyCharacterController::StopAiming(const FInputActionValue& value)
{
	IsAiming = false;
	AMyHUD* ControlledHUD = Cast<AMyHUD>(GetHUD());
	if (ControlledHUD && ControlledHUD->GetRoboAimUI())
	{
		ControlledHUD->GetRoboAimUI()->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AMyCharacterController::UpdateAimDirection(const FInputActionValue& value)
{
	if (IsAiming)
	{
		FVector WorldLoc, WorldDir;
		if (DeprojectMousePositionToWorld(WorldLoc, WorldDir))
		{
			AimDirection = (WorldLoc - ControlledRobo->GetActorLocation()).GetSafeNormal();
			//화살표 ui에 aimdirection적용
		}
	}
}

void AMyCharacterController::SwitchWeaponInput(const FInputActionValue& value)
{
	CurrentWeaponIndex = (CurrentWeaponIndex + 1) % WeaponCount;
	AMyHUD* ControlledHUD = Cast<AMyHUD>(GetHUD());

	UE_LOG(LogTemp, Warning, TEXT("ControlledHUD: %s"), *GetNameSafe(ControlledHUD));
	UE_LOG(LogTemp, Warning, TEXT("RoboWeaponUI: %s"), *GetNameSafe(ControlledHUD ? ControlledHUD->GetRoboWeaponUI() : nullptr));

	if (ControlledHUD && ControlledHUD->GetRoboWeaponUI())
	{
		ControlledHUD->GetRoboWeaponUI()->PlaySwitchAnimation(CurrentWeaponIndex);
	}
}

void AMyCharacterController::UseToolInput(const FInputActionValue& value)
{
	//UI변경(아이템사용)
	//해당툴의몽타쥬실행
	//남은시간 UI노출
	//시간종료 시 원래 몽타쥬로 돌아가기, UI 사라짐
}

void AMyCharacterController::SwitchToolInput(const FInputActionValue& value)
{
}
 
//void AMyCharacterController::EquipInput(const FInputActionValue& value)
//{
//	//ControlledCharacter->PlayEquipWeaponMontage();
//}

void AMyCharacterController::InteractionInput(const FInputActionValue& value)
{
	/*if (ControlledCharacter)
		ControlledCharacter->InteractionAction();*/
}

void AMyCharacterController::OnSpaceStarted()
{
	SpacePressedTime = GetWorld()->GetTimeSeconds();
}

void AMyCharacterController::OnSpaceCompleted()
{
	float HeldTime = GetWorld()->GetTimeSeconds() - SpacePressedTime;
	AMyRobo* Robo = Cast<AMyRobo>(GetPawn());
	if (!Robo) return;

	//길게 눌렀을 때
	if (HeldTime >= HoldThreshold)
	{
		Robo->HandleLongPress();
	}
	//짧게 눌렀을 때
	else
	{
		Robo->HandleShortPress();
	}
}
