// Fill out your copyright notice in the Description page of Project Settings.


#include "JMH/BTSAlertBeam.h"
#include "NiagaraComponent.h"

// Sets default values
ABTSAlertBeam::ABTSAlertBeam()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Niagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));
	Niagara->SetupAttachment(RootComponent);

	bIsNiagaraActive = false;
}

// Called when the game starts or when spawned
void ABTSAlertBeam::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(FlickerTimerHandle, this, &ABTSAlertBeam::ToggleNiagaraSystem, FlickerInterval, true);
}

// Called every frame
void ABTSAlertBeam::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABTSAlertBeam::SetToShowStrataBeam(FLinearColor BeamColor, FVector BeamEnd)
{
	Niagara->SetVectorParameter(TEXT("User.Beam End"), BeamEnd);
	Niagara->SetColorParameter(TEXT("User.Color"), BeamColor);
	Niagara->Activate();
}

void ABTSAlertBeam::Deactivate()
{
	GetWorld()->GetTimerManager().ClearTimer(FlickerTimerHandle);
	Niagara->Deactivate();
}

void ABTSAlertBeam::ToggleNiagaraSystem()
{
	if (bIsNiagaraActive)
	{
		Niagara->Deactivate();
	}
	else
	{
		Niagara->Activate();
	}

	bIsNiagaraActive = !bIsNiagaraActive; // 상태를 반전
}
