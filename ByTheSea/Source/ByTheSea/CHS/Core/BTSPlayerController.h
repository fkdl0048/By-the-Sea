// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BTSPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BYTHESEA_API ABTSPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ABTSPlayerController();
	virtual void BeginPlay() override;
};
