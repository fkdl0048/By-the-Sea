#include "BTSGameMode.h"
#include "BTSGameState.h"
#include "BTSHUD.h"

ABTSGameMode::ABTSGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	GameStateClass = ABTSGameState::StaticClass();
	HUDClass = ABTSHUD::StaticClass();
	DefaultPawnClass = nullptr;
	CurrentGameState = EGameState::MainMenu;
	RespawnCount = 0;
}

void ABTSGameMode::StartGame()
{
	SetGameState(EGameState::Playing);
	RespawnPlayer();
	ABTSGameState* BTSGameState = GetGameState<ABTSGameState>();
	if (BTSGameState)
	{
		BTSGameState->CurrentPlayTime = 0.0f;
		UE_LOG(LogTemp, Warning, TEXT("Current Play Time: %f"), BTSGameState->CurrentPlayTime);
	}
	Cast<ABTSHUD>(GetWorld()->GetFirstPlayerController()->GetHUD())->ShowStartHUD();
	OnGameStart.Broadcast();
}

void ABTSGameMode::EndGame()
{
	SetGameState(EGameState::GameOver);
	ABTSGameState* BTSGameState = GetGameState<ABTSGameState>();
	if (BTSGameState)
	{
		BTSGameState->IncrementRespawnCount();
	}
	RespawnPlayer();
	OnGameOver.Broadcast();
}

void ABTSGameMode::ClearGame()
{
	SetGameState(EGameState::GameClear);
	ABTSGameState* BTSGameState = GetGameState<ABTSGameState>();
	if (BTSGameState)
	{
		BTSGameState->UpdateMaxTime();
		UE_LOG(LogTemp, Warning, TEXT("Max Play Time: %f"), BTSGameState->MaxPlayTime);
	}

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC->GetPawn())
	{
		PC->GetPawn()->Destroy();
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

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NiagaraSystem, PlayerSpawnLocation, FRotator::ZeroRotator, FVector(1.0f));
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
