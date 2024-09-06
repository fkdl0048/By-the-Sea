// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerInputComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BYTHESEA_API UPlayerInputComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPlayerInputComponent();

	virtual void BeginPlay() override;	
	virtual void InitializeComponent() override;
	virtual void SetupInputComponent(UInputComponent* InputComponent);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
protected:
	UFUNCTION()
	virtual void OnLanded(const FHitResult& Hit);

	virtual void OnRegister() override;

	class ACharacter* OwnerCharacter;
	class APlayerController* OwnerController;

// IMC
public:
	FORCEINLINE class UInputMappingContext* GetIMC() { return InputMappingContext; };
protected:
	UPROPERTY()
	TObjectPtr<class UInputMappingContext> InputMappingContext;
};
