#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterTypes.h"
#include "SlashCharacter.generated.h"

// Forward declarations
class UInputMappingContext;
class UInputAction;
class UCharacterMovementHandler;
class UCharacterInteractionHandler;
class UCharacterAttackHandler;
class USpringArmComponent;
class UCameraComponent;
class UGroomComponent;
class AWeapon;

UCLASS()
class SLASH_API ASlashCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASlashCharacter();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);

protected:

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCharacterInteractionHandler* InteractionHandler;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCharacterMovementHandler* MovementHandler;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCharacterAttackHandler* AttackHandler;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputMappingContext* SlashMappingContext;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* MovementAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* LookingAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* ToggleWeaponAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* InteractionAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* RightMouseButtonAction;

	UPROPERTY(EditAnywhere, Category = Hair)
	UGroomComponent* Hair;

	UPROPERTY(EditAnywhere, Category = Hair)
	UGroomComponent* Eyebrows;

private:
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;

	EActionState ActionState = EActionState::EAS_Unoccupied;
	ECharacterArmedState CharacterArmedState = ECharacterArmedState::Unarmed;
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	UPROPERTY()
	AWeapon* EquippedWeapon;

public:
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }

	FORCEINLINE ECharacterArmedState GetArmedState() const { return CharacterArmedState; }
	void SetArmedState(ECharacterArmedState NewState) { CharacterArmedState = NewState; }

	FORCEINLINE EActionState GetActionState() const { return ActionState; }
	void SetActionState(EActionState NewState) { ActionState = NewState; }

	FORCEINLINE AWeapon* GetEquippedWeapon() const { return EquippedWeapon; }
	void SetEquippedWeapon(AWeapon* NewWeapon) { EquippedWeapon = NewWeapon; }
};
