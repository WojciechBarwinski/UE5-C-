#pragma once

#include "Characters/Handlers/CharacterAttackHandler.h"
#include "GameFramework/Character.h"
#include "Characters/SlashCharacter.h"

void UCharacterAttackHandler::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	checkf(OwnerCharacter.IsValid(), TEXT("OwnerCharacter is invalid!"));
	SlashCharacter = Cast<ASlashCharacter>(OwnerCharacter.Get());
	checkf(SlashCharacter.IsValid(), TEXT("SlashCharacter is invalid!"));
}

//public functions
void UCharacterAttackHandler::Attack(const FInputActionValue& Value)
{
	if (CanAttack())
	{
		PlayAttackMontage();
		SlashCharacter->SetActionState(EActionState::EAS_Attacking);
	}
}

void UCharacterAttackHandler::AttackEnd()
{
	SlashCharacter->SetActionState(EActionState::EAS_Unoccupied);
}

//private functions
void UCharacterAttackHandler::PlayAttackMontage()
{
	UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
	if (AnimInstance && AttackMontage) {

		AnimInstance->Montage_Play(AttackMontage);
		const int32 Selection = FMath::RandRange(0, 1);
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

bool UCharacterAttackHandler::CanAttack()
{
	return SlashCharacter->GetActionState() == EActionState::EAS_Unoccupied &&
		SlashCharacter->GetArmedState() == ECharacterArmedState::WeaponDrawn;
}