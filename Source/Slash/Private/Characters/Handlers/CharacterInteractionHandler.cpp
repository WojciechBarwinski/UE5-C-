#pragma once

#include "Characters/Handlers/CharacterInteractionHandler.h"
#include "GameFramework/Character.h"
#include "Interfaces/Attachable.h"
#include "Characters/SlashCharacter.h"
#include "Test/Openable.h"
#include "items/Weapons/Weapon.h"

void UCharacterInteractionHandler::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	checkf(OwnerCharacter.IsValid(), TEXT("OwnerCharacter is invalid!"));
	SlashCharacter = Cast<ASlashCharacter>(OwnerCharacter.Get());
	checkf(SlashCharacter.IsValid(), TEXT("SlashCharacter is invalid!"));
}

void UCharacterInteractionHandler::SheathedWeapon()
{
	IAttachable::Execute_SheathedWeapon(SlashCharacter->GetEquippedWeapon(), SlashCharacter->GetMesh());
}

void UCharacterInteractionHandler::DrawWeapon()
{
	IAttachable::Execute_DrawWeapon(SlashCharacter->GetEquippedWeapon(), SlashCharacter->GetMesh());
}

//public functions
void UCharacterInteractionHandler::Interaction(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Interaction pressed!"));
	TArray<AActor*> OverlappingActors;
	OwnerCharacter->GetOverlappingActors(OverlappingActors);

	if (!OverlappingActors.IsEmpty() && SlashCharacter->GetArmedState() == ECharacterArmedState::Unarmed)
	{
		for (AActor* Actor : OverlappingActors)
		{
			if (Actor->Implements<UAttachable>())
			{
				IAttachable::Execute_Attach(Actor, OwnerCharacter->GetMesh());

				if (AWeapon* Weapon = Cast<AWeapon>(Actor))
				{
					SlashCharacter->SetEquippedWeapon(Weapon);
					SlashCharacter->SetArmedState(ECharacterArmedState::WeaponDrawn);
				}

				//break;
			}
			else if (Actor->Implements<UOpenable>())
			{
				UE_LOG(LogTemp, Warning, TEXT("Openable actor found!"));
				IOpenable::Execute_Open(Actor, OwnerCharacter->GetMesh());
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
void UCharacterInteractionHandler::PlayDrawOrSheathedWeaponMontage(const FName SectionName)
{
	UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
	if (AnimInstance && EquipMontage) {
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
	}
}


