// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SlashCharacter.h"
#include "Components/InputComponent.h"
#include "EnhancedInputSubsystems.h" 
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GroomComponent.h"
#include "Items/Item.h"
//#include "Interfaces/Attachable.h"
#include "Items/Weapons/Weapon.h"

ASlashCharacter::ASlashCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bIsRightMouseButtonPressed = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

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

	hasSword = false;

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

void ASlashCharacter::MoveByCharacter(const FInputActionValue& Value)
{
	const FVector2D MoveValue = Value.Get<FVector2D>();

	FVector Forward = GetActorForwardVector();
	FVector Right = GetActorRightVector();

	AddMovementInput(Forward, MoveValue.Y);
	FRotator RotationDelta(0.f, MoveValue.X * RotatinSpeed, 0.f);
	AddActorLocalRotation(RotationDelta);
}

void ASlashCharacter::MoveByCamera(const FInputActionValue& Value)
{
	const FVector2D MoveVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MoveVector.Y);
	AddMovementInput(RightDirection, MoveVector.X);
	
}

void ASlashCharacter::FullMove(const FInputActionValue& Value)
{
	if (bIsRightMouseButtonPressed)
	{
		MoveByCamera(Value);
	}
	else
	{
		MoveByCharacter(Value);
	}
}

void ASlashCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisValue = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisValue.X);
	AddControllerPitchInput(LookAxisValue.Y);
}

void ASlashCharacter::Interaction(const FInputActionValue& Value)
{

	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		if (Actor->Implements<UAttachable>())
		{
			IAttachable::Execute_Attach(Actor, GetMesh(), FName("RightHandSocket")); // "WeaponSocket" to nazwa gniazda w skeletal meshu postaci
			break; // Przerywa po znalezieniu pierwszego attachable
		}
	}


	/*AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);
	if (OverlappingWeapon)
	{
		OverlappingWeapon->Equip(GetMesh(), FName("RightHandSocket"));
		hasSword = true;
	}*/
}

void ASlashCharacter::OnRightMouseButtonPressed(const FInputActionValue& Value)
{
	bIsRightMouseButtonPressed = true;
}

void ASlashCharacter::OnRightMouseButtonReleased(const FInputActionValue& Value)
{
	bIsRightMouseButtonPressed = false;
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

		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &ASlashCharacter::FullMove);
		EnhancedInputComponent->BindAction(LookingAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(InteractionAction, ETriggerEvent::Started, this, &ASlashCharacter::Interaction);
		
		// Podpiêcie akcji prawego przycisku myszy
		EnhancedInputComponent->BindAction(RightMouseButtonAction, ETriggerEvent::Started, this, &ASlashCharacter::OnRightMouseButtonPressed);
		EnhancedInputComponent->BindAction(RightMouseButtonAction, ETriggerEvent::Completed, this, &ASlashCharacter::OnRightMouseButtonReleased);

		
	}
}

