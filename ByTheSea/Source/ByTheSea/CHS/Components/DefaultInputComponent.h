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
};
