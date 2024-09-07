// Fill out your copyright notice in the Description page of Project Settings.

#include "BTSGameState.h"

ABTSGameState::ABTSGameState()
{
	CurrentPlayTime = 0.0f;
	MaxPlayTime = 0.0f;
	RespawnCount = 0;
}

void ABTSGameState::UpdatePlayTime(float DeltaTime)
{
	CurrentPlayTime += DeltaTime;
}

void ABTSGameState::UpdateMaxTime()
{
	if (CurrentPlayTime > MaxPlayTime)
	{
		MaxPlayTime = CurrentPlayTime;
	}
}

void ABTSGameState::IncrementRespawnCount()
{
	RespawnCount++;
}
