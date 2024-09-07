// Fill out your copyright notice in the Description page of Project Settings.


#include "JMH/BirdManager.h"

#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ABirdManager::ABirdManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InnerCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("InnerCapsule"));
	InnerCapsule->SetupAttachment(RootComponent);
	OuterCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("OuterCapsule"));
	OuterCapsule->SetupAttachment(RootComponent);
	CurrentSpawnCount = 0;
}

// Called when the game starts or when spawned
void ABirdManager::BeginPlay()
{
	Super::BeginPlay();
	// DrawDebugCylinder()
	// 물고기 찾기
	// 물고기 배열 선언
	TArray<AActor*> FoundFish;

	// 월드에서 모든 ABTSCharacterPlayer 액터를 찾음
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABTSCharacterPlayer::StaticClass(), FoundFish);

	// 찾은 액터들을 처리
	for (AActor* Actor : FoundFish)
	{
		ABTSCharacterPlayer* FishCharacter = Cast<ABTSCharacterPlayer>(Actor);
		if (FishCharacter)
		{
			// FishCharacter에 원하는 로직 수행
			Fish = FishCharacter;
		}
	}
}

// Called every frame
void ABirdManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DistanceFromPlayer = UE::Geometry::Distance(Fish->GetActorLocation(), this->GetActorLocation());
	// 범위 안에 들어오면 
	if(DistanceFromPlayer >= InnerRadius && DistanceFromPlayer <= OuterRadius)
	{
		// SpawnCount마리까지만 스폰 가능
		if(CurrentSpawnCount > SpawnCount) return;
		
		// 갈매기 생성
		CurrentSpawnCount++;
		SpawnBird();		
	}

}

void ABirdManager::SpawnBird()
{
	// Set Spawn Collision Handling Override
	FActorSpawnParameters ActorSpawnParams;	
	// ActorSpawnParams.Owner = GetOwner();
	// ActorSpawnParams.Instigator = InstigatorPawn;
	ABTSBird* Seagull = nullptr;
	
	CalculateSpawnTransform();
	// 서버에서 생성하면 자동 리플리케이션
	Seagull = GetWorld()->SpawnActor<ABTSBird>(BirdClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
	Seagull->ShowAlertRay(SpawnRotation);
	// Seagull->ShowAlertRay(Fish);
	if(!Seagull)
	{
		UE_LOG(LogTemp, Log, TEXT("Seagull is Null"))
	}
}

void ABirdManager::CalculateSpawnTransform()
{
	//Fish.Get
	FVector UpVector = FVector(0,0,1);
	float RandomRange = FMath::FRandRange(-CornAngle, CornAngle);
	FRotator RandomRotator = FRotator(RandomRange,0,RandomRange);

	// 플레이어 -> 갈매기 방향
	// 갈매기는 이 반대로 ray를 쏜다. 
	FVector SpawnDirection = UKismetMathLibrary::GreaterGreater_VectorRotator(UpVector,RandomRotator);
	FVector SpawnPoint = Fish->GetActorLocation() + (SpawnDirection * CornRadius);
	
	SpawnLocation = SpawnPoint;
	FVector TMPVec = Fish->GetActorLocation() - SpawnPoint;
	TMPVec.Normalize();
	SpawnRotation = TMPVec.Rotation();
	// SpawnRotation.Yaw -= 90;
	//SpawnRotation = FRotator(-SpawnDirection.Rotation().Pitch, -SpawnDirection.Rotation().Yaw, -SpawnDirection.Rotation().Roll);

}

