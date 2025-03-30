//#include "Characters/Handlers/CharacterInteractionHandler.h"
#include "Characters/Handlers/CharacterMovementHandler.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"

void UCharacterMovementHandler::BeginPlay()
{
    Super::BeginPlay();
    OwnerCharacter = Cast<ACharacter>(GetOwner());

    checkf(OwnerCharacter.IsValid(), TEXT("OwnerCharacter is invalid!"));
	CharacterRotationSpeed = 5.f;
}

//public functions
void UCharacterMovementHandler::Look(const FInputActionValue& Value)
{
    if (!CheckController()) return;

        const FVector2D LookAxisValue = Value.Get<FVector2D>();
        OwnerCharacter->AddControllerYawInput(LookAxisValue.X);
        OwnerCharacter->AddControllerPitchInput(LookAxisValue.Y);
}

void UCharacterMovementHandler::HandlerMove(const FInputActionValue& Value)
{
    if (!CheckController()) return;

    if (bIsRightMouseButtonPressed)
    {
        MoveByCamera(Value);
    }
    else
    {
        MoveByCharacter(Value);
    }
}

//private functions
void UCharacterMovementHandler::MoveByCharacter(const FInputActionValue& Value)
{
    const FVector2D MoveValue = Value.Get<FVector2D>();

    FVector Forward = OwnerCharacter->GetActorForwardVector();
    FVector Right = OwnerCharacter->GetActorRightVector();

    OwnerCharacter->AddMovementInput(Forward, MoveValue.Y);
    FRotator RotationDelta(0.f, MoveValue.X * CharacterRotationSpeed, 0.f);
    OwnerCharacter->AddActorLocalRotation(RotationDelta);
}

void UCharacterMovementHandler::MoveByCamera(const FInputActionValue& Value)
{
    const FVector2D MoveVector = Value.Get<FVector2D>();

    const FRotator Rotation = OwnerCharacter->GetController()->GetControlRotation();
    const FRotator YawRotation(0, Rotation.Yaw, 0);

    const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

    OwnerCharacter->AddMovementInput(ForwardDirection, MoveVector.Y);
    OwnerCharacter->AddMovementInput(RightDirection, MoveVector.X);

}

bool UCharacterMovementHandler::CheckController() const
{
    return OwnerCharacter.IsValid() && OwnerCharacter->GetController();
}

