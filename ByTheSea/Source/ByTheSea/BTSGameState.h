// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "BTSGameState.generated.h"

/**
 * 
 */
UCLASS()
class BYTHESEA_API ABTSGameState : public AGameState
{
	GENERATED_BODY()

public:
	ABTSGameState();

	UFUNCTION(BlueprintCallable, Category = "Game")
	void UpdatePlayTime(float DeltaTime);

	UFUNCTION(BlueprintCallable, Category = "Game")
	void UpdateMaxTime();

	UFUNCTION(BlueprintCallable, Category = "Game")
	void IncrementRespawnCount();

	UPROPERTY(BlueprintReadOnly, Category = "Game")
	float CurrentPlayTime;

	UPROPERTY(BlueprintReadOnly, Category = "Game")
	float MaxPlayTime;

	UPROPERTY(BlueprintReadOnly, Category = "Game")
	int32 RespawnCount;
};
