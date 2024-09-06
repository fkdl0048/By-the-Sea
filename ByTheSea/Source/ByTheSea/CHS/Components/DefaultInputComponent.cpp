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
}

void UDefaultInputComponent::SetupInputComponent(UInputComponent* InputComponent)
{
	Super::SetupInputComponent(InputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &UDefaultInputComponent::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &UDefaultInputComponent::Look);
}

void UDefaultInputComponent::Move(const FInputActionValue& Value)
{
	if (!IsValid(OwnerCharacter)) return;
	if (!IsValid(OwnerController)) return;

	FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = OwnerController->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = UKismetMathLibrary::GetForwardVector(YawRotation);
	const FVector RightDirection = UKismetMathLibrary::GetRightVector(YawRotation);

	// 플레이어 이동 입력 처리
	OwnerCharacter->AddMovementInput(ForwardDirection, MovementVector.Y);
	OwnerCharacter->AddMovementInput(RightDirection, MovementVector.X);
}

void UDefaultInputComponent::Look(const FInputActionValue& Value)
{
	if (!IsValid(OwnerCharacter)) return;
	if (!IsValid(OwnerController)) return;

	FVector2D LookVector = Value.Get<FVector2D>();
	OwnerCharacter->AddControllerYawInput(LookVector.X * MouseSensitivity);
	OwnerCharacter->AddControllerPitchInput(LookVector.Y * MouseSensitivity);
}
