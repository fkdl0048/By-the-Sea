// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BTSHUD.generated.h"

/**
 * 
 */
UCLASS()
class BYTHESEA_API ABTSHUD : public AHUD
{
	GENERATED_BODY()

public:
	ABTSHUD();

	UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
	void ShowStartHUD();

	UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
	void ShowGameOverHUD();

	UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
	void ShowClearHUD();

	UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
	void UpdateTimeHUD(float CurrentTime);
};
