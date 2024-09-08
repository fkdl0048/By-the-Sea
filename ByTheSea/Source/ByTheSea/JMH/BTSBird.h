// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BTSAlertBeam.h"
#include "GameFramework/Actor.h"
#include "BTSBird.generated.h"

UCLASS()
class BYTHESEA_API ABTSBird : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABTSBird();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void ShowAlertRay(FRotator DirToFly);

	UFUNCTION(BlueprintCallable)
	void FlyToPlayer(FRotator DirToFly);
	
	UFUNCTION()
	void SetProjectileActive(bool IsActive);	

public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<class USceneComponent> SceneComponent;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UBoxComponent> CollisionComp;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> BirdMesh;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovementComponent;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ABTSAlertBeam> AlertBeamClass;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ABTSAlertBeam> AlertBeam;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sound)
	TObjectPtr<class USoundBase> MySound;
public:
	// 어딘가에 박힌 경우 true
	bool bCrashed;

	UPROPERTY(EditDefaultsOnly)
	float AttackDelay;
	
	UPROPERTY(EditDefaultsOnly)
	float Speed;

	UPROPERTY(EditDefaultsOnly)
	int32 FlickeringCount;
};
