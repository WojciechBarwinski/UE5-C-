// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <InputActionValue.h>
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterTypes.h"
#include "SlashCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class UCharacterMovementHandler;
class USpringArmComponent;
class UCameraComponent;
class UGroomComponent;
class AItem;
class UAnimMontage;
class AWeapon;

UCLASS()
class SLASH_API ASlashCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	ASlashCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	virtual void BeginPlay() override;

	UPROPERTY()
	UCharacterMovementHandler* MovementHandler;

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
	UInputAction* InteractionAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* RightMouseButtonAction;

	UPROPERTY(EditAnywhere, Category = Hair)
	UGroomComponent* Hair;

	UPROPERTY(EditAnywhere, Category = Hair)
	UGroomComponent* Eyebrows;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float CharacterRotationSpeed = 10.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float MovementSpeedMultiplier = 1.0f;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	bool HasWeapon; 

	void Attack(const FInputActionValue& Value);
	void Interaction(const FInputActionValue& Value);
	void StoreOrDrawWeapon();

	void PlayEquipMontage(FName SectionName);
	void PlayAttackMontage();
private:
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;

	bool bIsRightMouseButtonPressed;

	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* EquipMontage;

	UFUNCTION(BlueprintCallable)
	void AttackEnd();
	bool CanAttack();
	bool CanDisarm();
	bool CanArm();

public:
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
};
