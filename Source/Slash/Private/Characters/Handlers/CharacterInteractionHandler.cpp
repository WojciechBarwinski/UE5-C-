#pragma once

#include "Characters/Handlers/CharacterInteractionHandler.h"
#include "GameFramework/Character.h"
#include "Interfaces/Attachable.h"
#include "Characters/SlashCharacter.h"

void UCharacterInteractionHandler::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	checkf(OwnerCharacter.IsValid(), TEXT("OwnerCharacter is invalid!"));
	SlashCharacter = Cast<ASlashCharacter>(OwnerCharacter.Get());
	checkf(SlashCharacter.IsValid(), TEXT("SlashCharacter is invalid!"));
}

//public functions
void UCharacterInteractionHandler::Interaction(const FInputActionValue& Value)
{
	TArray<AActor*> OverlappingActors;
	OwnerCharacter->GetOverlappingActors(OverlappingActors);

	if (!OverlappingActors.IsEmpty() && SlashCharacter->GetArmedState() == ECharacterArmedState::Unarmed)
	{
		for (AActor* Actor : OverlappingActors)
		{
			if (Actor->Implements<UAttachable>())
			{
				IAttachable::Execute_Attach(Actor, OwnerCharacter->GetMesh());
				//tmp without mechanic for 1h/2h weapon CharacterState = IAttachable::Execute_GetCharacterState(Actor);
				SlashCharacter->SetArmedState(ECharacterArmedState::WeaponDrawn);
				break;
			}
		}
	}
}

void UCharacterInteractionHandler::DrawOrSheathedWeapon()
{
	//if (CanSheathedWeapon()) {
	if (SlashCharacter->GetArmedState() == ECharacterArmedState::WeaponDrawn) {
		PlayDrawOrSheathedWeaponMontage(FName("SheathedWeapon"));
		SlashCharacter->SetArmedState(ECharacterArmedState::WeaponSheathed);
	}
	//else if (CanDrawWeapon()) {
	else if (SlashCharacter->GetArmedState() == ECharacterArmedState::WeaponSheathed) {
		PlayDrawOrSheathedWeaponMontage(FName("DrawWeapon"));
		SlashCharacter->SetArmedState(ECharacterArmedState::WeaponDrawn);
	}
}

//private functions
void UCharacterInteractionHandler::PlayDrawOrSheathedWeaponMontage(FName SectionName)
{
	UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
	if (AnimInstance && EquipMontage) {
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
	}
}

