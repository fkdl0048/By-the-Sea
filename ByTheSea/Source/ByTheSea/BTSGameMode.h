// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "NiagaraFunctionLibrary.h"
#include "BTSGameMode.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameStart);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameOver);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameClear);

UENUM(BlueprintType)
enum class EGameState : uint8
{
	MainMenu,
	Playing,
	GameOver,
	GameClear
};

UCLASS()
class BYTHESEA_API ABTSGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ABTSGameMode();

	UFUNCTION(BlueprintCallable, Category = "Game")
	void StartGame();

	UFUNCTION(BlueprintCallable, Category = "Game")
	void EndGame();

	UFUNCTION(BlueprintCallable, Category = "Game")
	void ClearGame();

	UFUNCTION(BlueprintCallable, Category = "Game")
	void RespawnPlayer();

	UPROPERTY(BlueprintAssignable, Category = "Game")
	FOnGameStart OnGameStart;

	UPROPERTY(BlueprintAssignable, Category = "Game")
	FOnGameOver OnGameOver;

	UPROPERTY(BlueprintAssignable, Category = "Game")
	FOnGameClear OnGameClear;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game")
	TSubclassOf<APawn> PlayerPawnClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game")
	FVector PlayerSpawnLocation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game")
	UNiagaraSystem* NiagaraSystem;

private:
	EGameState CurrentGameState;
	int32 RespawnCount;

	void SetGameState(EGameState NewState);
};
