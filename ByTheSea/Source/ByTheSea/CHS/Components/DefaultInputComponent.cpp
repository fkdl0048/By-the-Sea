// Fill out your copyright notice in the Description page of Project Settings.


#include "CHS/Components/DefaultInputComponent.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "InputModifiers.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "CHS/Character/BTSCharacterPlayer.h"

UDefaultInputComponent::UDefaultInputComponent()
{
}

void UDefaultInputComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UDefaultInputComponent::OnLanded(const FHitResult& Hit)
{
	Super::OnLanded(Hit);
}

void UDefaultInputComponent::InitializeComponent()
{
	Super::InitializeComponent();

	// Move 액션
	{
		MoveAction = NewObject<UInputAction>(this);
		check(MoveAction);
		MoveAction->ValueType = EInputActionValueType::Axis2D;

		TObjectPtr<UInputModifierSwizzleAxis> SwizzleModifer = NewObject<UInputModifierSwizzleAxis>();
		TObjectPtr<UInputModifierNegate> NegateModifer = NewObject<UInputModifierNegate>();

		{
			FEnhancedActionKeyMapping& EnhancedActionKeyMapping = InputMappingContext->MapKey(MoveAction, EKeys::W);
			EnhancedActionKeyMapping.Modifiers.Add(SwizzleModifer);
		}

		{
			FEnhancedActionKeyMapping& EnhancedActionKeyMapping = InputMappingContext->MapKey(MoveAction, EKeys::S);
			EnhancedActionKeyMapping.Modifiers.Add(SwizzleModifer);
			EnhancedActionKeyMapping.Modifiers.Add(NegateModifer);
		}

		{
			FEnhancedActionKeyMapping& EnhancedActionKeyMapping = InputMappingContext->MapKey(MoveAction, EKeys::A);
			EnhancedActionKeyMapping.Modifiers.Add(NegateModifer);
		}

		InputMappingContext->MapKey(MoveAction, EKeys::D);
	}

	// Look 액션
	{
		LookAction = NewObject<UInputAction>(this);
		check(LookAction);
		LookAction->ValueType = EInputActionValueType::Axis2D;

		TObjectPtr<UInputModifierNegate> LookNegateModifer = NewObject<UInputModifierNegate>();
		LookNegateModifer->bX = false;
		LookNegateModifer->bY = true;
		LookNegateModifer->bZ = false;
		LookAction->Modifiers.Add(LookNegateModifer);

		InputMappingContext->MapKey(LookAction, EKeys::Mouse2D);
	}

	// Launch 액션
	{
		LaunchAction = NewObject<UInputAction>(this);
		check(LaunchAction);
		LaunchAction->ValueType = EInputActionValueType::Boolean;

		TObjectPtr<UInputTriggerPressed> PressedTrigger = NewObject<UInputTriggerPressed>();
		TObjectPtr<UInputTriggerReleased> ReleasedTrigger = NewObject<UInputTriggerReleased>();

		FEnhancedActionKeyMapping& EnhancedActionKeyMapping = InputMappingContext->MapKey(LaunchAction, EKeys::SpaceBar);
		EnhancedActionKeyMapping.Triggers.Add(PressedTrigger);
		EnhancedActionKeyMapping.Triggers.Add(ReleasedTrigger);
	}
}

void UDefaultInputComponent::SetupInputComponent(UInputComponent* InputComponent)
{
	Super::SetupInputComponent(InputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &UDefaultInputComponent::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &UDefaultInputComponent::Look);
	EnhancedInputComponent->BindAction(LaunchAction, ETriggerEvent::Triggered, this, &UDefaultInputComponent::Launch);
}

void UDefaultInputComponent::Move(const FInputActionValue& Value)
{
	if (!IsValid(OwnerCharacter)) return;
	if (!IsValid(OwnerController)) return;

	FVector2D MovementVector = Value.Get<FVector2D>();

	if (bAllowMove)
	{
		const FRotator Rotation = OwnerController->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = UKismetMathLibrary::GetForwardVector(YawRotation);
		const FVector RightDirection = UKismetMathLibrary::GetRightVector(YawRotation);

		// 플레이어 이동 입력 처리
		OwnerCharacter->AddMovementInput(ForwardDirection, MovementVector.Y);
		OwnerCharacter->AddMovementInput(RightDirection, MovementVector.X);
		return;
	}
	
	ABTSCharacterPlayer* BTSCharacterPlayer = Cast<ABTSCharacterPlayer>(OwnerCharacter);
	if (!IsValid(BTSCharacterPlayer)) return;
	if (BTSCharacterPlayer->GetCurState() != ECharacterPlayerState::IDLE) return;

	ACharacter* PlayerCharacter = Cast<ACharacter>(GetOwner());
	check(PlayerCharacter);

	FRotator ControllerRotation = OwnerController->GetControlRotation();
	FRotator ControllerRotationYaw = FRotator(0.0f, ControllerRotation.Yaw, 0.0f);
	FVector ControllerFowardVector = UKismetMathLibrary::GetForwardVector(ControllerRotationYaw);
	FVector ControllerRightVector = UKismetMathLibrary::GetRightVector(ControllerRotationYaw);

	FVector InputDirection = (ControllerFowardVector * MovementVector.Y) + (ControllerRightVector * MovementVector.X);
	InputDirection.Normalize();

	FRotator NewRotation = InputDirection.Rotation();
	NewRotation.Roll += 90.0f;
	OwnerCharacter->GetMesh()->SetWorldRotation(NewRotation);
}

void UDefaultInputComponent::Look(const FInputActionValue& Value)
{
	if (!IsValid(OwnerCharacter)) return;
	if (!IsValid(OwnerController)) return;

	FVector2D LookVector = Value.Get<FVector2D>();
	OwnerCharacter->AddControllerYawInput(LookVector.X * MouseSensitivity);
	OwnerCharacter->AddControllerPitchInput(LookVector.Y * MouseSensitivity);
}

void UDefaultInputComponent::Launch(const FInputActionValue& Value)
{
	if (!IsValid(OwnerCharacter)) return;

	ABTSCharacterPlayer* BTSCharacterPlayer = Cast<ABTSCharacterPlayer>(OwnerCharacter);
	if (!IsValid(BTSCharacterPlayer)) return;
	if (BTSCharacterPlayer->GetCurState() != ECharacterPlayerState::IDLE) return;
		
	bool bJump = Value.Get<bool>();
	if (bJump)
	{
		// Jump Pressed
		LaunchPressedTime = GetWorld()->GetTime().GetWorldTimeSeconds();
	}
	else
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(OwnerController->GetLocalPlayer());
		if (!IsValid(Subsystem)) 0.0f;

		TArray<FKey> MoveActionMappedKey = Subsystem->QueryKeysMappedToAction(MoveAction);
		check(MoveActionMappedKey.Num() == 4);

		FVector CurInputVector = FVector::Zero();
		if (OwnerController->IsInputKeyDown(MoveActionMappedKey[0])) CurInputVector.Y += 1.0f;
		if (OwnerController->IsInputKeyDown(MoveActionMappedKey[1])) CurInputVector.Y -= 1.0f;
		if (OwnerController->IsInputKeyDown(MoveActionMappedKey[2])) CurInputVector.X -= 1.0f;
		if (OwnerController->IsInputKeyDown(MoveActionMappedKey[3])) CurInputVector.X += 1.0f;

		ACharacter* PlayerCharacter = Cast<ACharacter>(GetOwner());
		check(PlayerCharacter);

		FRotator ControllerRotation = OwnerController->GetControlRotation();
		FRotator ControllerRotationYaw = FRotator(0.0f, ControllerRotation.Yaw, 0.0f);
		FVector ControllerFowardVector = UKismetMathLibrary::GetForwardVector(ControllerRotationYaw);
		FVector ControllerRightVector = UKismetMathLibrary::GetRightVector(ControllerRotationYaw);

		FVector InputDirection = (ControllerFowardVector * CurInputVector.Y) + (ControllerRightVector * CurInputVector.X);
		InputDirection.Normalize();

		double LaunchReleasedTime = GetWorld()->GetTime().GetWorldTimeSeconds();
		double TimeSpan = LaunchReleasedTime - LaunchPressedTime;
		TimeSpan = FMath::Clamp(TimeSpan, 0.0, LaunchPressTimeMax);
		double AdditionalChargeLaunchStrength = (TimeSpan / LaunchPressTimeMax) * (LaunchStrengthMax - LaunchStrengthMin);
		double CalculatedLaunchStrength = LaunchStrengthMin + AdditionalChargeLaunchStrength;

		UE_LOG(LogTemp, Log, TEXT("TimeSpan : %lf, CalculatedLaunchStrengh : %lf"), TimeSpan, CalculatedLaunchStrength);

		FVector LaunchVector = InputDirection + SpeedCorretion;
		LaunchVector *= CalculatedLaunchStrength;

		// 물고기 날리기
		OwnerCharacter->LaunchCharacter(LaunchVector, true, true);

		// Flutter로 설정하기
		UFunction* FlutterFunction = OwnerCharacter->FindFunction(FName(TEXT("EventSetFlutter")));
		if (FlutterFunction)
		{
			OwnerCharacter->ProcessEvent(FlutterFunction, nullptr);
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("SetFlutterFunc Not Found"))
		}
	}
}