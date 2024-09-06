// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CHS/Character/BTSCharacterBase.h"
#include "BTSCharacterPlayer.generated.h"

/**
 * 
 */
UCLASS()
class BYTHESEA_API ABTSCharacterPlayer : public ABTSCharacterBase
{
	GENERATED_BODY()
public:
	ABTSCharacterPlayer();

	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

// 카메라 부분
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	TObjectPtr<class UCameraComponent> FollowCamera;
};
