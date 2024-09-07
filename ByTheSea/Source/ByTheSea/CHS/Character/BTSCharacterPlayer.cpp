// Fill out your copyright notice in the Description page of Project Settings.


#include "CHS/Character/BTSCharacterPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "CHS/Components/PlayerInputComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

ABTSCharacterPlayer::ABTSCharacterPlayer()
{
	// ĳ���� �⺻ ����
	{
		bUseControllerRotationPitch = false;
		bUseControllerRotationYaw = true;
		bUseControllerRotationRoll = false;
	}

	// ī�޶� �κ�
	{
		CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
		CameraBoom->SetupAttachment(RootComponent);
		CameraBoom->TargetArmLength = 600.0f;
		CameraBoom->bUsePawnControlRotation = true;

		FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
		FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
		FollowCamera->bUsePawnControlRotation = false;
	}

	// �޽� & �ִϸ��̼�
	{
		/*static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(
			TEXT(""));
		if (CharacterMeshRef.Object)
		{
			GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
		}*/

		GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FRotator(0.0f, -90.0f, 0.0f));
	}
}

void ABTSCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();
}

void ABTSCharacterPlayer::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	if (CurState == ECharacterPlayerState::WAITFORIDLE)
	{
		CurState = ECharacterPlayerState::IDLE;
	}
}

void ABTSCharacterPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	APlayerController* CurController = Cast<APlayerController>(NewController);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(CurController->GetLocalPlayer());
	if (!IsValid(Subsystem)) return;

	Subsystem->ClearAllMappings();
}

void ABTSCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	TSet<UActorComponent*> ActorComponents = GetComponents();

	for (UActorComponent* ActorComponent : ActorComponents)
	{
		UPlayerInputComponent* CustomInputComponent = Cast<UPlayerInputComponent>(ActorComponent);
		if (CustomInputComponent)
		{
			CustomInputComponent->SetupInputComponent(PlayerInputComponent);
		}
	}
}
