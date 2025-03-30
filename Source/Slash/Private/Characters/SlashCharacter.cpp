#pragma once

#include "Characters/SlashCharacter.h"
#include "EnhancedInputSubsystems.h" 
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GroomComponent.h"
#include "Characters/CharacterTypes.h"
#include "Characters/CharacterMovementHandler.h"
#include "Characters/Handlers/CharacterInteractionHandler.h"
#include "Characters/Handlers/CharacterAttackHandler.h"

ASlashCharacter::ASlashCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	MovementHandler = CreateDefaultSubobject<UCharacterMovementHandler>(TEXT("MovementHandler"));
	InteractionHandler = CreateDefaultSubobject<UCharacterInteractionHandler>(TEXT("InteractionHandler"));
	AttackHandler = CreateDefaultSubobject<UCharacterAttackHandler>(TEXT("AttackHandler"));

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 300.f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	ViewCamera->SetupAttachment(CameraBoom);

	Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	Hair->SetupAttachment(GetMesh());
	Hair->AttachmentName = FString("head");

	Eyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrows"));
	Eyebrows->SetupAttachment(GetMesh());
	Eyebrows->AttachmentName = FString("head");
}

void ASlashCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController())) {

		if (UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			subsystem->AddMappingContext(SlashMappingContext, 0);
		}
	}
}

void ASlashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(InteractionAction, ETriggerEvent::Started, InteractionHandler, &UCharacterInteractionHandler::Interaction);
		EnhancedInputComponent->BindAction(ToggleWeaponAction, ETriggerEvent::Started, InteractionHandler, &UCharacterInteractionHandler::DrawOrSheathedWeapon);
	
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, AttackHandler, &UCharacterAttackHandler::Attack);

		EnhancedInputComponent->BindAction(LookingAction, ETriggerEvent::Triggered, MovementHandler, &UCharacterMovementHandler::Look);
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, MovementHandler, &UCharacterMovementHandler::HandlerMove);
		EnhancedInputComponent->BindAction(RightMouseButtonAction, ETriggerEvent::Started, MovementHandler, &UCharacterMovementHandler::OnRightMouseButtonPressed);
		EnhancedInputComponent->BindAction(RightMouseButtonAction, ETriggerEvent::Completed, MovementHandler, &UCharacterMovementHandler::OnRightMouseButtonReleased);
	}
}

