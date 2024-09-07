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