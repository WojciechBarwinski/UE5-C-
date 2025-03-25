// Fill out your copyright notice in the Description page of Project Settings.


#include "items/Weapons/Weapon.h"
#include "Characters/SlashCharacter.h"



void AWeapon::Equip(USceneComponent* InParent, FName InSocketName)
{
    FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
    ItemMesh->AttachToComponent(InParent, TransformRules, InSocketName);
}

void AWeapon::Attach_Implementation(USceneComponent* InParent, FName InSocketName)
{
    if (InParent && ItemMesh)
    {
        FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
        ItemMesh->AttachToComponent(InParent, TransformRules, InSocketName);
    }
}

void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}
