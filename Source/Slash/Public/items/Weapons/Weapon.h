#pragma once

#include "CoreMinimal.h"
#include "items/Item.h"
#include "Interfaces/Attachable.h"
#include "Weapon.generated.h"

class USoundBase;
class UBoxComponent;


UCLASS()
class SLASH_API AWeapon : public AItem, public IAttachable
{
	GENERATED_BODY()

public:
	AWeapon();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties")
	ECharacterState WeaponType;

	//void Equip(USceneComponent* InParent, FName InSocketName);
	virtual void Attach_Implementation(USceneComponent* InParent) override;
	virtual ECharacterState GetCharacterState_Implementation() const override;
	virtual void SheathedWeapon_Implementation(USceneComponent* InParent) override;
	virtual void DrawWeapon_Implementation(USceneComponent* InParent) override;

	void ClearIgnoredActors() { IgnoredActors.Empty(); }
	
protected:
	virtual void BeginPlay() override;

	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION(BlueprintImplementableEvent)
	void CreateFields(const FVector& FieldLocation);

	
private:
	FName GetSocketNameBasedOnState() const;
	void PlayEquipSound() const;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	USoundBase* EquipSound;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	UBoxComponent* WeaponBox;
	
	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceStart;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceEnd;

	TSet<TWeakObjectPtr<AActor>> IgnoredActors;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	float Damage = 20.f;
public:
	FORCEINLINE UBoxComponent* GetWeaponBox() const { return WeaponBox; }
};
