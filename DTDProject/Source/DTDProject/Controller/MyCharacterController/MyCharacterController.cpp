// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/MyCharacterController/MyCharacterController.h"
#include "Engine/World.h"
#include "MyRobo/MyRobo.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Interface/InteractionObject.h"
#include "Interface/UINavigateInterface.h"
#include "Blueprint/UserWidget.h"
#include "UI/MainUI.h"
#include "UI/RoboAimUI.h"
#include "Components/Image.h"
#include "ActorComponent/InventoryComponent.h"
#include "ActorComponent/StateComponent/RoboComponent.h"

AMyCharacterController::AMyCharacterController()
{
	static ConstructorHelpers::FClassFinder<UMainUI> MainWidgetFinder(TEXT("/Game/BluePrint/UI/BP_MainUI.BP_MainUI_C"));
	if (MainWidgetFinder.Succeeded())
	{
		MainWidgetClass = MainWidgetFinder.Class;
	}

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
	static ConstructorHelpers::FObjectFinder<UInputAction> InteractionActionFinder(TEXT("/Script/EnhancedInput.InputAction'/Game/BluePrint/MyRobo/Input/IA_SpacePress.IA_SpacePress'"));
	if (InteractionActionFinder.Succeeded())
	{
		InteractionAction = InteractionActionFinder.Object;
	}
	/*static ConstructorHelpers::FObjectFinder<UInputAction> EquipActionFinder(TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprints/MyRobo/Input/IA_Equip_Ch.IA_Equip_Ch'"));
	if (EquipActionFinder.Succeeded())
	{
		EquipAction = EquipActionFinder.Object;
	}*/

	static ConstructorHelpers::FObjectFinder<UInputAction> NavigateUpActionFinder(TEXT("/Script/EnhancedInput.InputAction'/Game/BluePrint/MyRobo/Input/IA_NavigateUp.IA_NavigateUp'"));
	if (NavigateUpActionFinder.Succeeded())
	{
		NavigateUpAction = NavigateUpActionFinder.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> NavigateDownActionFinder(TEXT("/Script/EnhancedInput.InputAction'/Game/BluePrint/MyRobo/Input/IA_NavigateDown.IA_NavigateDown'"));
	if (NavigateDownActionFinder.Succeeded())
	{
		NavigateDownAction = NavigateDownActionFinder.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> SelectUIButtonActionFinder(TEXT("/Script/EnhancedInput.InputAction'/Game/BluePrint/MyRobo/Input/IA_SelectUIButton.IA_SelectUIButton'"));
	if (SelectUIButtonActionFinder.Succeeded())
	{
		SelectUIButtonAction = SelectUIButtonActionFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> GameInputMappingContextFinder(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/BluePrint/MyRobo/Input/IMC_Character.IMC_Character'"));
	if (GameInputMappingContextFinder.Succeeded())
	{
		GameMappingContext = GameInputMappingContextFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> UIInputMappingContextFinder(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/BluePrint/MyRobo/Input/IMC_UI.IMC_UI'"));
	if (UIInputMappingContextFinder.Succeeded())
	{
		UIMappingContext = UIInputMappingContextFinder.Object;
	}

	/*static ConstructorHelpers::FObjectFinder<UInputAction> InteractionActionFinder(TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprints/MyRobo/Input/IA_Interaction_Ch.IA_Interaction_Ch'"));
	if (InteractionActionFinder.Succeeded())
		InteractionAction = InteractionActionFinder.Object;*/

	PrimaryActorTick.bCanEverTick = true;
}

void AMyCharacterController::BeginPlay()
{
	Super::BeginPlay();

	ControlledRobo = Cast<AMyRobo>(GetCharacter());

	if (MainWidgetClass)
	{
		MainWidgetInstance = CreateWidget<UMainUI>(this, MainWidgetClass);
		if (MainWidgetInstance)
		{
			MainWidgetInstance->AddToViewport();

			if (AMyRobo* MyRobo = Cast<AMyRobo>(GetPawn()))
			{
				MyRobo->SetupMainUIReference(MainWidgetInstance);
				MyRobo->BroadcastCurrentWeaponStates();
			}
		}
	}
}

void AMyCharacterController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*FHitResult HitResult;
	bool isHit = UKismetSystemLibrary::BoxTraceSingle(this,
		ControlledRobo->GetActorLocation(),
		ControlledRobo->GetActorLocation() + ControlledRobo->GetActorForwardVector() * 500.0f,
		FVector(20.0f, 20.0f, 50.0f),
		FRotator::ZeroRotator,
		UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel7),
		false,
		{ ControlledRobo },
		EDrawDebugTrace::ForOneFrame,
		HitResult,
		true
	);
	if (isHit)
		ControlledRobo->SetInteractionObject(Cast<IInteractionObject>(HitResult.GetActor()));
	else
		ControlledRobo->SetInteractionObject(nullptr);*/
}

void AMyCharacterController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (UEnhancedInputLocalPlayerSubsystem* InputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		InputSystem->AddMappingContext(GameMappingContext, 0);
	}

	if (UEnhancedInputComponent* input = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// 기존 입력 액션 바인딩
		input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyCharacterController::MoveInput);
		input->BindAction(MoveAction, ETriggerEvent::Completed, this, &AMyCharacterController::MoveEndInput);
		input->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyCharacterController::LookInput);
		input->BindAction(DashAction, ETriggerEvent::Started, this, &AMyCharacterController::DashInput);
		input->BindAction(MeleeAttackAction, ETriggerEvent::Started, this, &AMyCharacterController::MeleeAttackInput);
		input->BindAction(RangedAttackAction, ETriggerEvent::Started, this, &AMyCharacterController::StartAiming); //우클릭 시작 시
		input->BindAction(RangedAttackAction, ETriggerEvent::Completed, this, &AMyCharacterController::StopAiming); //우클릭 끝 시
		input->BindAction(SwitchWeaponAction, ETriggerEvent::Started, this, &AMyCharacterController::SwitchWeaponInput);
		input->BindAction(UseToolAction, ETriggerEvent::Started, this, &AMyCharacterController::UseToolInput);
		input->BindAction(SwitchToolAction, ETriggerEvent::Started, this, &AMyCharacterController::SwitchToolInput);
		input->BindAction(InteractionAction, ETriggerEvent::Started, this, &AMyCharacterController::InteractionStarted);
		input->BindAction(InteractionAction, ETriggerEvent::Completed, this, &AMyCharacterController::InteractionCompleted);
		/*EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Started, this, &AMyCharacterController::EquipInput);*/

		// UI 네비게이션 입력 액션 바인딩
		input->BindAction(NavigateUpAction, ETriggerEvent::Triggered, this, &AMyCharacterController::OnNavigateUp);
		input->BindAction(NavigateDownAction, ETriggerEvent::Triggered, this, &AMyCharacterController::OnNavigateDown);
		input->BindAction(SelectUIButtonAction, ETriggerEvent::Triggered, this, &AMyCharacterController::OnSelectUIButton);
	}
}

void AMyCharacterController::MoveInput(const FInputActionValue& value)
{
	isMoveInput = true;
	IsAttacking = true;
	FVector2D MoveValue = value.Get<FVector2D>();
	if (ControlledRobo)
	{
		ControlledRobo->AddMovementInput(GetTransformComponent()->GetForwardVector(), MoveValue.X);
		ControlledRobo->AddMovementInput(GetTransformComponent()->GetRightVector(), MoveValue.Y);
	}
}

void AMyCharacterController::MoveEndInput(const FInputActionValue& value)
{
	isMoveInput = false;
	IsAttacking = false;
}

void AMyCharacterController::LookInput(const FInputActionValue& value)
{
	FVector2D MoveValue = value.Get<FVector2D>();

	if (!IsAiming)
	{
		AddYawInput(MoveValue.X);
		AddPitchInput(MoveValue.Y);
	}
	else
	{
		if (MainWidgetInstance)
		{
			MainWidgetInstance->UpdateAimPos(MoveValue * 5.0f);
		}
	}
}

void AMyCharacterController::DashInput(const FInputActionValue& value)
{
	if (ControlledRobo)
	{
		ControlledRobo->LaunchCharacter(ControlledRobo->GetActorForwardVector() * 800, true, true);
	}
}

void AMyCharacterController::MeleeAttackInput(const FInputActionValue& value)
{
	if (IsAttacking) return;

	if (ControlledRobo)
	{
		if (IsAiming)
		{
			if(MainWidgetInstance)
			{
				FVector2D ScreenPosition = MainWidgetInstance->GetCrosshairScreenPosition();

				IsAttacking = true;
				ControlledRobo->PerformAttack(ScreenPosition);
			}
			
		}
		else
		{   //근접 공격
			IsAttacking = true;
			ControlledRobo->PerformAttack();
		}
	}
}

void AMyCharacterController::StartAiming(const FInputActionValue& value)
{
	IsAiming = true;
	IsAttacking = false;

	if (MainWidgetInstance)
	{
		MainWidgetInstance->StartAiming();
    }

	if (ControlledRobo)
	{
		ControlledRobo->StartRangedAim();
	}

	// 마우스 커서 숨기거나 포커스 고정할 필요가 있으면 여기서 처리
}

void AMyCharacterController::StopAiming(const FInputActionValue& value)
{
	IsAiming = false;

	if (MainWidgetInstance)
	{
		MainWidgetInstance->StopAiming();
	}

	if (IsAttacking)
	{
		return;
	}
	else
	{
		if (ControlledRobo)
		{
			ControlledRobo->StopRangedAim();
		}
	}
}

void AMyCharacterController::SwitchWeaponInput(const FInputActionValue& value)
{
	if (MainWidgetInstance)
	{
		MainWidgetInstance->PlayWeaponSwitchAnimation(CurrentWeaponIndex);
	}

	CurrentWeaponIndex = (CurrentWeaponIndex + 1) % WeaponCount;

	if (ControlledRobo)
	{
		ControlledRobo->SwitchActiveRangedWeapon();
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
	if (ControlledRobo && ControlledRobo->GetInventoryComponent())
	{
		ControlledRobo->GetInventoryComponent()->SwitchActiveTool();
	}
}

void AMyCharacterController::InteractionStarted(const FInputActionValue& value)
{
	if (AMyRobo* Robo = Cast<AMyRobo>(GetPawn()))
	{
		Robo->StartSpaceHold();
	}
}

void AMyCharacterController::InteractionCompleted(const FInputActionValue& value)
{
	if (AMyRobo* Robo = Cast<AMyRobo>(GetPawn()))
	{
		Robo->StopSpaceHold();
	}
}

void AMyCharacterController::OnNavigateUp()
{
	/*UE_LOG(LogTemp, Error, TEXT("AMyCharacterController::OnNavigateUp --- INPUT ACTION FIRED!"));
	if (UMainUI* MainUI = Cast<UMainUI>(MainWidgetInstance))
	{
		UUserWidget* CurrentWidget = MainUI->GetCurrentWidget();
		if (CurrentWidget && CurrentWidget->Implements<UUINavigateInterface>())
		{
			UE_LOG(LogTemp, Warning, TEXT("AMyCharacterController::OnNavigateUp --- Executing  interface call on %s"), *CurrentWidget->GetName());
			IUINavigateInterface::Execute_NavigateUp(CurrentWidget);
		}
	}*/
}

void AMyCharacterController::OnNavigateDown()
{
	/*if (UMainUI* MainUI = Cast<UMainUI>(MainWidgetInstance))
	{
		UUserWidget* CurrentWidget = MainUI->GetCurrentWidget();
		if (CurrentWidget && CurrentWidget->Implements<UUINavigateInterface>())
		{
			IUINavigateInterface::Execute_NavigateDown(CurrentWidget);
		}
	}*/
}

void AMyCharacterController::OnSelectUIButton()
{
	/*if (UMainUI* MainUI = Cast<UMainUI>(MainWidgetInstance))
	{
		UUserWidget* CurrentWidget = MainUI->GetCurrentWidget();
		if (CurrentWidget && CurrentWidget->Implements<UUINavigateInterface>())
		{
			IUINavigateInterface::Execute_Select(CurrentWidget);
		}
	}*/
}

bool AMyCharacterController::FireStartPostion(FVector& WorldPosition, FVector& WorldDirection)
{
	UImage* AimUI = MainWidgetInstance->GetRoboAimUI()->GetImageArrow();
	FVector2D ScreenPosition = AimUI->GetCachedGeometry().GetAbsolutePosition();
	TSharedPtr<SWidget> AimWidget = AimUI->GetCachedWidget();
	if (AimWidget.IsValid())
	{
		FWidgetPath Path;
		TSharedPtr<SWindow> Window = FSlateApplication::Get().FindWidgetWindow(AimWidget.ToSharedRef());
		if (Window.IsValid())
		{
			FVector2D WindowLeftTopPosition = Window->GetPositionInScreen();
			ScreenPosition -= WindowLeftTopPosition;
			DeprojectScreenPositionToWorld(ScreenPosition.X, ScreenPosition.Y, WorldPosition, WorldDirection);
			return true;
		}
	}
	return false;
}

