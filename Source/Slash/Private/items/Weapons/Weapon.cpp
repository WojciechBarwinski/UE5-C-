// Fill out your copyright notice in the Description page of Project Settings.


#include "items/Weapons/Weapon.h"
#include "Characters/SlashCharacter.h"



//void AWeapon::Equip(USceneComponent* InParent, FName InSocketName)
//{
//    FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
//    ItemMesh->AttachToComponent(InParent, TransformRules, InSocketName);
//}

void AWeapon::Attach_Implementation(USceneComponent* InParent)
{

    ASlashCharacter* SlashCharacter = Cast<ASlashCharacter>(InParent->GetOwner());
    
    if (InParent && ItemMesh)
    {
        FName SocketName = GetSocketNameBasedOnState();
        FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
        ItemMesh->AttachToComponent(InParent, TransformRules, SocketName);
        ItemState = EItemState::EIS_Equipped;
    }
}

ECharacterState AWeapon::GetCharacterState_Implementation() const
{
    return WeaponType;
}

void AWeapon::SheathedWeapon_Implementation(USceneComponent* InParent)
{
    FName SocketName = "SpineSocket";
    FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
    ItemMesh->AttachToComponent(InParent, TransformRules, SocketName);
}

void AWeapon::DrawWeapon_Implementation(USceneComponent* InParent)
{
    FName SocketName = GetSocketNameBasedOnState();
    FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
    ItemMesh->AttachToComponent(InParent, TransformRules, SocketName);
}

void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

FName AWeapon::GetSocketNameBasedOnState() const
{
    switch (IAttachable::Execute_GetCharacterState(this)) {
    case ECharacterState::ECS_EquippedOneHandedWeapon: return "RightHandSocket";
    case ECharacterState::ECS_EquippedTwoHandedWeapon: return "RightHand_For2h_Socket";
    default: return "RightHandSocket";
    }
}
