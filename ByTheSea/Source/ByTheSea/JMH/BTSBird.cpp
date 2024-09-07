// Fill out your copyright notice in the Description page of Project Settings.


#include "JMH/BTSBird.h"

#include "CHS/Character/BTSCharacterPlayer.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ABTSBird::ABTSBird()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(SceneComponent);
	
	BirdMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BirdMesh"));
	BirdMesh->SetupAttachment(SceneComponent);	
 	
	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComp"));
	CollisionComp->SetupAttachment(BirdMesh);
	
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	
	bCrashed = false;
}

// Called when the game starts or when spawned
void ABTSBird::BeginPlay()
{
	Super::BeginPlay();
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ABTSBird::OnBoxBeginOverlap);

	// Ready
	// ShowAlertRay();
}

// Called every frame
void ABTSBird::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	/*if(!bCrashed)
	{
		// 공격 
		
		// 한번 박히면 끝
		bCrashed = true;
	}*/

}

void ABTSBird::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABTSCharacterPlayer* Fish = Cast<ABTSCharacterPlayer>(OtherActor);
	if(Fish)
	{
		UE_LOG(LogTemp, Warning, TEXT("You Die"))
	}
	else
	{
		
	}
	// 정지
	// 콜리전 끄기
	// 자지가신 무시
	
	UE_LOG(LogTemp, Warning, TEXT("OtherActor : %s"), *OtherActor->GetName())
	SetProjectileActive(false);
	ProjectileMovementComponent->bSimulationEnabled = false;
	ProjectileMovementComponent->Velocity = FVector::ZeroVector;
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 속도 0으로 만들고 정지
	Speed= 0;	 
	ProjectileMovementComponent->StopMovementImmediately();
	
	
}

void ABTSBird::ShowAlertRay(FRotator DirToFly)
{
	FTimerHandle AttackTimerHandle;
	// Ray쏘기
	// 타이머
	// 일정 시간 후 FlyToPlayer() 호출
	GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle,
		[this, DirToFly]
	{
		ABTSBird::FlyToPlayer(DirToFly);
	}, AttackDelay, false);
}

void ABTSBird::FlyToPlayer(FRotator DirToFly)
{
	// 방향을 전달받아서 발사
	ProjectileMovementComponent->Velocity = DirToFly.Vector() * Speed;
}

void ABTSBird::SetProjectileActive(bool IsActive)
{
	SetActorEnableCollision(IsActive); 
	SetActorTickEnabled(IsActive);
	/*if (!IsActive)
	{
		ProjectileMovementComponent->Velocity = FVector::ZeroVector;
		// CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else
	{
		ProjectileMovementComponent->Velocity = GetActorForwardVector() * 100.0f;
	}*/
}
