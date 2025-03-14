// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <InputActionValue.h>
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SlashCharacter.generated.h"


class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class UGroomComponent;


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

	UPROPERTY(EditAnywhere, Category = Input)
	UInputMappingContext* SlashMappingContext;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* MovementAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* LookingAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* RightMouseButtonAction;

	UPROPERTY(EditAnywhere, Category = Hair)
	UGroomComponent* Hair;

	UPROPERTY(EditAnywhere, Category = Hair)
	UGroomComponent* Eyebrows;

	UPROPERTY(EditAnywhere)
	float RotatinSpeed;

	void MoveByCharacter(const FInputActionValue& Value);
	void MoveByCamera(const FInputActionValue& Value);
	void FullMove(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void OnRightMouseButtonPressed(const FInputActionValue& Value);
	void OnRightMouseButtonReleased(const FInputActionValue& Value);

private:
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;

	bool bIsRightMouseButtonPressed;
};
