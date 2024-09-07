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
	// UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), WeaponData.EjectShellParticles, EjectLocation, EjectRotation);
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

void ABTSAlertBeam::FlickerBeam(uint32 InMaxRepeatCount)
{
	Niagara->Activate();
	// const int InitialRepeatCount = 0;
	// FlickerContinuously(InitialRepeatCount, InMaxRepeatCount);
	GetWorld()->GetTimerManager().SetTimer(
		FlickerTimerHandle,
		[this]()
		{
			Niagara->Deactivate();
		},FlickerInterval, false);

	GetWorld()->GetTimerManager().SetTimer(
		FlickerTimerHandle,
		[this]()
		{
			Niagara->Activate();
		},FlickerInterval * 2, false);
	
	GetWorld()->GetTimerManager().SetTimer(
		FlickerTimerHandle,
		[this]()
		{
			Niagara->Deactivate();
		},FlickerInterval * 3, false);
}

void ABTSAlertBeam::FlickerContinuously(int32 InCurRepeatCount, int32 InMaxRepeatCount)
{
	if (InCurRepeatCount >= InMaxRepeatCount)
	{
		GetWorld()->GetTimerManager().ClearTimer(FlickerTimerHandle);
		return;
	}

	Flicker();
	
	GetWorld()->GetTimerManager().SetTimer(
		FlickerTimerHandle,
		[this, InCurRepeatCount, InMaxRepeatCount]()
		{			
			FlickerContinuously(InCurRepeatCount + 1, InMaxRepeatCount);
		},FlickerInterval, false);
}

// 이 함수를 FlickerInterval*2 초마다 반복한다. 
void ABTSAlertBeam::Flicker()
{
	if(Niagara->IsActive())
	{
		Niagara->Deactivate();		
	}
	else
	{
		Niagara->Activate();		
	}
	// FlickerInterval초 뒤에 꺼짐
	
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
