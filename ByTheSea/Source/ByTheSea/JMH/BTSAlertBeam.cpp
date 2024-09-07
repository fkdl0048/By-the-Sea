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

	FlickerInterval = 0.5f;
}

// Called when the game starts or when spawned
void ABTSAlertBeam::BeginPlay()
{
	Super::BeginPlay();
		
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
	const int InitialRepeatCount = 0;
	FlickerContinuously(InitialRepeatCount, InMaxRepeatCount);
	
}

void ABTSAlertBeam::FlickerContinuously(int32 InCurRepeatCount, int32 InMaxRepeatCount)
{
	if (InCurRepeatCount >= InMaxRepeatCount)
	{
		GetWorld()->GetTimerManager().ClearTimer(FlickerTimerHandle);
		return;
	}

	// Flicker();
	Niagara->Activate();
	
	GetWorld()->GetTimerManager().SetTimer(
		FlickerTimerHandle,
		[this, InCurRepeatCount, InMaxRepeatCount]()
		{
			FlickerContinuously(InCurRepeatCount + 1, InMaxRepeatCount);
		},
		FlickerInterval, false);
}

// 이 함수를 FlickerInterval*2 초마다 반복한다. 
void ABTSAlertBeam::Flicker()
{
	Niagara->Activate();
	// FlickerInterval초 뒤에 꺼짐
	GetWorld()->GetTimerManager().SetTimer(FlickerTimerHandle, [this]()
	{
		Niagara->Deactivate();	
	}, FlickerInterval, false);
}

void ABTSAlertBeam::Deactivate()
{
	Niagara->Deactivate();
}
