#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Characters/CharacterTypes.h"
#include "CharacterInteractionHandler.generated.h"

class ASlashCharacter;
class AWeapon;
struct FInputActionValue;

UCLASS()
class SLASH_API UCharacterInteractionHandler : public UActorComponent
{
	GENERATED_BODY()

public:	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* EquipMontage;

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void PutWeaponOnBack();

	void Interaction(const FInputActionValue& Value);
	void DrawOrSheathedWeapon();
	
protected:
	virtual void BeginPlay() override;

private:

	TWeakObjectPtr<ACharacter> OwnerCharacter;
	TWeakObjectPtr<ASlashCharacter> SlashCharacter;
	void PlayDrawOrSheathedWeaponMontage(FName SectionName);
};
