// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SlashAnimInstance.h"
#include "Characters/SlashCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"


void USlashAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

    SlashCharacter = Cast<ASlashCharacter>(TryGetPawnOwner());
    if (SlashCharacter)
    {
        SlashCharacterMovement = SlashCharacter->GetCharacterMovement();
    }
}

void USlashAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

    //SlashCharacter = Cast<ASlashCharacter>(TryGetPawnOwner());
    if (SlashCharacter && bHasSword != SlashCharacter->hasSword)
    {
        bHasSword = SlashCharacter->hasSword;
        // Mo�esz nawet wywo�a� r�czne od�wie�enie blend�w
    }

    if (SlashCharacterMovement)
    {
        GroundSpeed = UKismetMathLibrary::VSizeXY(SlashCharacterMovement->Velocity);
        IsFalling = SlashCharacterMovement->IsFalling();
    }
}
