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
	bSpawned = false;
	bDoOnce = false;
	
}

// Called when the game starts or when spawned
void ABirdManager::BeginPlay()
{
	Super::BeginPlay();
	// 게임 모드를 가져온다
	ABTSGameMode* GameMode = Cast<ABTSGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		// OnGameStart 델리게이트에 FindFish 함수를 바인딩
		GameMode->OnGameStart.AddDynamic(this, &ABirdManager::FindFish);

		// 게임 오버시 갈매기 냅두고		
		GameMode->OnGameOver.AddDynamic(this, &ABirdManager::FindFish);

		// 게임 클리어 시 갈매기 모두 제거
		GameMode->OnGameClear.AddDynamic(this,&ABirdManager::DeleteAllBird);
	}
}

// Called every frame
void ABirdManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(!Fish) return; 
		
	DistanceFromPlayer = UE::Geometry::Distance(Fish->GetActorLocation(), this->GetActorLocation());
	// 범위 안에 들어오면 
	if(DistanceFromPlayer >= InnerRadius && DistanceFromPlayer <= OuterRadius)
	{
		UE_LOG(LogTemp, Log, TEXT("In Boundary"))
		if(bDoOnce == false)
		{
			bDoOnce = true;
			SpawnBird();
		}
		
		if(bSpawned == false)
		{
			bSpawned = true;
			UE_LOG(LogTemp, Log, TEXT("bSpawned"))
			// 스폰 다 되고 6초 뒤에 스폰
			GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle,this,&ABirdManager::SpawnBird,
			SpawnInterval, false);			
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Out Boundary"))
		bSpawned = false;
		// 구역 밖이면 Timer 초기화
		GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
	}
	UE_LOG(LogTemp, Log, TEXT("bSpawned: %d"), bSpawned)
}

void ABirdManager::FindFish()
{
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

void ABirdManager::SpawnBird()
{
	bSpawned = false;
	GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
	SpawnBirdsInSequence(3, 0.3f); // 0.3초 간격으로 3번 스폰
}

void ABirdManager::SpawnBirdsInSequence(int32 SpawnCnt, float Interval)
{
	// 타이머 설정으로 반복 스폰 실행
	GetWorld()->GetTimerManager().SetTimer(SpawnSequenceTimerHandle, this, &ABirdManager::SpawnSingleBird, 
		Interval, true); // 'true'로 반복 설정
    
	BirdsToSpawn = SpawnCnt; // 스폰 횟수를 저장
	SpawnedBirdCount = 0; // 스폰된 새의 초기 카운트를 설정
}

void ABirdManager::SpawnSingleBird()
{
	if (SpawnedBirdCount >= BirdsToSpawn)
	{
		// 타이머 중지
		GetWorld()->GetTimerManager().ClearTimer(SpawnSequenceTimerHandle);
		return;
	}
    
	// 스폰 로직
	FActorSpawnParameters ActorSpawnParams;    
	ABTSBird* Seagull = nullptr;
    
	CalculateSpawnTransform();
    
	Seagull = GetWorld()->SpawnActor<ABTSBird>(BirdClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
	Seagull->ShowAlertRay(SpawnRotation);
	AllBirds.Add(Seagull);
    
	if (!Seagull)
	{
		UE_LOG(LogTemp, Log, TEXT("Seagull is Null"));
	}
    
	SpawnedBirdCount++; // 스폰 카운트 증가
}


/*
void ABirdManager::SpawnBird()
{
	bSpawned = false;
	GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
	// Set Spawn Collision Handling Override
	FActorSpawnParameters ActorSpawnParams;	
	ABTSBird* Seagull = nullptr;
	
	CalculateSpawnTransform();

	Seagull = GetWorld()->SpawnActor<ABTSBird>(BirdClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
	Seagull->ShowAlertRay(SpawnRotation);
	AllBirds.Add(Seagull);
	
	if(!Seagull)
	{
		UE_LOG(LogTemp, Log, TEXT("Seagull is Null"))
	}
}
*/

void ABirdManager::DeleteAllBird()
{
	// 완전 초기화
	for (auto item : AllBirds)
	{
		item->Destroy();
	}
	AllBirds.Empty();
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

