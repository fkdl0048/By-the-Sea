#include "CHS/Components/PlayerInputComponent.h"
#include "GameFramework/Character.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"

UPlayerInputComponent::UPlayerInputComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;

}

void UPlayerInputComponent::BeginPlay()
{
	Super::BeginPlay();

	SetIsReplicated(true);
}

void UPlayerInputComponent::InitializeComponent()
{
	Super::InitializeComponent();

	InputMappingContext = NewObject<UInputMappingContext>(this);
}

void UPlayerInputComponent::SetupInputComponent(UInputComponent* InputComponent)
{
	OwnerController = Cast<APlayerController>(OwnerCharacter->GetController());
	if (!IsValid(OwnerController)) return;

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(OwnerController->GetLocalPlayer());
	if (!IsValid(Subsystem)) return;

	Subsystem->AddMappingContext(InputMappingContext, 0);
}

void UPlayerInputComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void UPlayerInputComponent::OnLanded(const FHitResult& Hit)
{
}

void UPlayerInputComponent::OnRegister()
{
	Super::OnRegister();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!IsValid(OwnerCharacter)) DestroyComponent();

	OwnerCharacter->LandedDelegate.AddUniqueDynamic(this, &UPlayerInputComponent::OnLanded);
}

