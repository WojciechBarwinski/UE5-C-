#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterMovementHandler.generated.h"

class ACharacter;
struct FInputActionValue;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SLASH_API UCharacterMovementHandler : public UActorComponent
{
    GENERATED_BODY()

public:
    void Look(const FInputActionValue& Value);
    void FullMove(const FInputActionValue& Value);
	void OnRightMouseButtonPressed(const FInputActionValue& Value) { bIsRightMouseButtonPressed = true; }
	void OnRightMouseButtonReleased(const FInputActionValue& Value) { bIsRightMouseButtonPressed = false; }

    void SetRotationSpeed(float Speed) { CharacterRotationSpeed = Speed; }

protected:
    virtual void BeginPlay() override;

private:

    TWeakObjectPtr<ACharacter> OwnerCharacter;
    float CharacterRotationSpeed;
    bool bIsRightMouseButtonPressed = false;

    void MoveByCharacter(const FInputActionValue& Value);
    void MoveByCamera(const FInputActionValue& Value);
    bool CheckController() const;
};