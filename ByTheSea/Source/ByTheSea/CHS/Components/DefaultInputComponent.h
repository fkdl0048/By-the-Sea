// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CHS/Components/PlayerInputComponent.h"
#include "DefaultInputComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BYTHESEA_API UDefaultInputComponent : public UPlayerInputComponent
{
	GENERATED_BODY()
public:
	UDefaultInputComponent();

	virtual void BeginPlay() override;
	virtual void OnLanded(const FHitResult& Hit);
	virtual void InitializeComponent() override;
	virtual void SetupInputComponent(UInputComponent* InputComponent) override;

// 이동
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 bAllowMove : 1;

	UPROPERTY()
	TObjectPtr<class UInputAction> MoveAction;

	UFUNCTION()
	virtual void Move(const FInputActionValue& Value);

// 카메라 회전
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MouseSensitivity;

	UPROPERTY()
	TObjectPtr<class UInputAction> LookAction;

	UFUNCTION()
	virtual void Look(const FInputActionValue& Value);

// 물고기 날리기
protected:
	UPROPERTY()
	TObjectPtr<class UInputAction> LaunchAction;

	UPROPERTY()
	double LaunchPressedTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	double LaunchPressTimeMax = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float LaunchStrengthMax;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float LaunchStrengthMin;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector SpeedCorretion = { 0.0f, 0.0f, 1.0f };

	UFUNCTION()
	virtual void Launch(const FInputActionValue& Value);
};
