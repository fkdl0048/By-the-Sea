#include "BTSGameMode.h"
#include "BTSGameState.h"
#include "BTSHUD.h"

ABTSGameMode::ABTSGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	GameStateClass = ABTSGameState::StaticClass();
	HUDClass = ABTSHUD::StaticClass();
	CurrentGameState = EGameState::MainMenu;
	RespawnCount = 0;
}

void ABTSGameMode::StartGame()
{
	SetGameState(EGameState::Playing);
	RespawnPlayer();
	Cast<ABTSHUD>(GetWorld()->GetFirstPlayerController()->GetHUD())->ShowStartHUD();
	OnGameStart.Broadcast();
}

void ABTSGameMode::EndGame()
{
	SetGameState(EGameState::GameOver);
	Cast<ABTSHUD>(GetWorld()->GetFirstPlayerController()->GetHUD())->ShowGameOverHUD();
	OnGameOver.Broadcast();
}

void ABTSGameMode::ClearGame()
{
	SetGameState(EGameState::GameClear);
	ABTSGameState* BTSGameState = GetGameState<ABTSGameState>();
	if (BTSGameState)
	{
		BTSGameState->UpdateMaxTime();
	}
	Cast<ABTSHUD>(GetWorld()->GetFirstPlayerController()->GetHUD())->ShowClearHUD();
	OnGameClear.Broadcast();
}

void ABTSGameMode::RespawnPlayer()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	APawn* NewPawn = GetWorld()->SpawnActor<APawn>(PlayerPawnClass, PlayerSpawnLocation, FRotator::ZeroRotator, SpawnParams);
	if (NewPawn)
	{
		APlayerController* PC = GetWorld()->GetFirstPlayerController();
		if (PC)
		{
			PC->Possess(NewPawn);
		}
	}

	ABTSGameState* BTSGameState = GetGameState<ABTSGameState>();
	if (BTSGameState)
	{
		BTSGameState->IncrementRespawnCount();
	}
}

void ABTSGameMode::SetGameState(EGameState NewState)
{
	CurrentGameState = NewState;
}
