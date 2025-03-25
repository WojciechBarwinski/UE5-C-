// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "items/Item.h"
#include "Interfaces/Attachable.h"
#include "Weapon.generated.h"

/**
 * 
 */
UCLASS()
class SLASH_API AWeapon : public AItem, public IAttachable
{
	GENERATED_BODY()

public:
	void Equip(USceneComponent* InParent, FName InSocketName);

	// Implementacja interfejsu
	virtual void Attach_Implementation(USceneComponent* InParent, FName InSocketName) override;

protected:

	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
};
