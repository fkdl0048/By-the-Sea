// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BTSBird.h"
#include "CHS/Character/BTSCharacterPlayer.h"
#include "ByTheSea/BTSGameMode.h"
#include "GameFramework/Actor.h"
#include "BirdManager.generated.h"

UCLASS()
class BYTHESEA_API ABirdManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABirdManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void FindFish();
public:
	void SpawnBird();
	void SpawnBirdsInSequence(int32 SpawnCnt, float Interval);

	void SpawnSingleBird();
	
	UFUNCTION()
	void DeleteAllBird();

	UFUNCTION()
	void ClearActivate();

	UFUNCTION()
	void ClearTimer();

	void CalculateSpawnTransform();
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<class UCapsuleComponent> InnerCapsule;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<class UCapsuleComponent> OuterCapsule;

	UPROPERTY()
	TObjectPtr<ABTSCharacterPlayer> Fish;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ABTSBird> BirdClass;

	UPROPERTY()
	TArray<ABTSBird*> AllBirds;

public:
	UPROPERTY(EditDefaultsOnly)
	float InnerRadius;

	UPROPERTY(EditDefaultsOnly)
	float OuterRadius;

	UPROPERTY(EditDefaultsOnly)
	float CornAngle;
	
	UPROPERTY(EditDefaultsOnly)
	int32 SpawnCount;
	
	UPROPERTY(EditDefaultsOnly)
	float CornRadius;

	UPROPERTY(EditDefaultsOnly)
	float SpawnInterval;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float DistanceFromPlayer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector SpawnLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FRotator SpawnRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 CurrentSpawnCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bSpawned;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bDoOnce;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bClear;

	FTimerHandle SpawnTimerHandle;
	FTimerHandle SpawnSequenceTimerHandle;

	UPROPERTY(EditDefaultsOnly)
	int32 BirdsToSpawn;
	
	UPROPERTY(EditDefaultsOnly)
	int32 SpawnedBirdCount;
	
};
