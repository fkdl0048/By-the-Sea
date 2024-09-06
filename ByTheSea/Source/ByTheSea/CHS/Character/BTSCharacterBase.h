// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BTSCharacterBase.generated.h"

UCLASS()
class BYTHESEA_API ABTSCharacterBase : public ACharacter
{
	GENERATED_BODY()
public:
	ABTSCharacterBase();
protected:
	virtual void BeginPlay() override;
};
