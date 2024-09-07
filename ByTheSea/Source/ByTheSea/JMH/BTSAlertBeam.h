// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BTSAlertBeam.generated.h"

UCLASS()
class BYTHESEA_API ABTSAlertBeam : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABTSAlertBeam();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetToShowStrataBeam(FLinearColor BeamColor, FVector BeamEnd);
	void FlickerBeam(uint32 InMaxRepeatCount);
	void FlickerContinuously(int32 InCurRepeatCount, int32 InMaxRepeatCount);
	void Flicker();
	void Deactivate();
public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UNiagaraComponent> Niagara;

	UPROPERTY(EditDefaultsOnly)
	float FlickerInterval;
	
	FTimerHandle FlickerTimerHandle;
};
