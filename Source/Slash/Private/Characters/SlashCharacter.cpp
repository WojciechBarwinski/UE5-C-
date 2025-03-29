// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SlashCharacter.h"
#include "Components/InputComponent.h"
#include "EnhancedInputSubsystems.h" 
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GroomComponent.h"
#include "Items/Item.h"
#include "Characters/CharacterTypes.h"
#include "Animation/AnimMontage.h"
#include "Items/Weapons/Weapon.h"
#include "Characters/CharacterMovementHandler.h"

ASlashCharacter::ASlashCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bIsRightMouseButtonPressed = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	MovementHandler = CreateDefaultSubobject<UCharacterMovementHandler>(TEXT("MovementHandler"));

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

	HasWeapon = false;
}

void ASlashCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController())) {

		if (UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			subsystem->AddMappingContext(SlashMappingContext, 0);
		}
	}

	if (MovementHandler)
	{
		MovementHandler->SetRotationSpeed(CharacterRotationSpeed);
	}
}

void ASlashCharacter::Attack(const FInputActionValue& Value)
{
	if (CanAttack())
	{
		PlayAttackMontage();
		ActionState = EActionState::EAS_Attacking;
	}
}

bool ASlashCharacter::CanAttack()
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState != ECharacterState::ECS_Unequipped;
}

bool ASlashCharacter::CanDisarm()
{
	return ActionState == EActionState::EAS_Unoccupied && 
		CharacterState != ECharacterState::ECS_Unequipped;
}

bool ASlashCharacter::CanArm()
{
	return //ActionState == EActionState::EAS_Unoccupied && 
		CharacterState == ECharacterState::ECS_Unequipped;
}

void ASlashCharacter::Interaction(const FInputActionValue& Value)
{
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors);

	if (HasWeapon)
	{
		StoreOrDrawWeapon();
		return;
	}

	if (!OverlappingActors.IsEmpty() && !HasWeapon)
	{
		for (AActor* Actor : OverlappingActors)
		{
			if (Actor->Implements<UAttachable>())
			{
				FString Name = Actor->GetActorNameOrLabel();
				UE_LOG(LogTemp, Warning, TEXT("Dodanie stanu: %s"), *Name);
			IAttachable::Execute_Attach(Actor, GetMesh());
			CharacterState = IAttachable::Execute_GetCharacterState(Actor);
			HasWeapon = true;
			break;
			}
		}
	}
}

void ASlashCharacter::StoreOrDrawWeapon()
{
	if (CanDisarm()) {
		PlayEquipMontage(FName("Unequip"));
		CharacterState = ECharacterState::ECS_Unequipped;
	} else if (CanArm()) {
		PlayEquipMontage(FName("Equip"));
		CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	}
}

void ASlashCharacter::PlayEquipMontage(FName SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && EquipMontage) {
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
	}
}

void ASlashCharacter::PlayAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AttackMontage) {

		AnimInstance->Montage_Play(AttackMontage);
		const int32 Selection = FMath::RandRange(0, 2);
		FName SectionName = FName();

		switch (Selection)
		{
		case 0:
			SectionName = FName("Attack1");
			break;
		case 1:
			SectionName = FName("Attack2");
			break;
		case 2:
			SectionName = FName("Attack3");
			break;
		default:
			break;
		}
		AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
	}
}

void ASlashCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void ASlashCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASlashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(LookingAction, ETriggerEvent::Triggered, MovementHandler, &UCharacterMovementHandler::Look);
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, MovementHandler, &UCharacterMovementHandler::FullMove);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(InteractionAction, ETriggerEvent::Started, this, &ASlashCharacter::Interaction);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ASlashCharacter::Attack);
		
		EnhancedInputComponent->BindAction(RightMouseButtonAction, ETriggerEvent::Started, MovementHandler, &UCharacterMovementHandler::OnRightMouseButtonPressed);
		EnhancedInputComponent->BindAction(RightMouseButtonAction, ETriggerEvent::Completed, MovementHandler, &UCharacterMovementHandler::OnRightMouseButtonReleased);
	}
}

