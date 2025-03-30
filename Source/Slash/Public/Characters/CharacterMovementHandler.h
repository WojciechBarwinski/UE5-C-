#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterMovementHandler.generated.h"

struct FInputActionValue;

UCLASS()
class SLASH_API UCharacterMovementHandler : public UActorComponent
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float CharacterRotationSpeed;

    void Look(const FInputActionValue& Value);
    void HandlerMove(const FInputActionValue& Value);
	void OnRightMouseButtonPressed(const FInputActionValue& Value) { bIsRightMouseButtonPressed = true; }
	void OnRightMouseButtonReleased(const FInputActionValue& Value) { bIsRightMouseButtonPressed = false; }

protected:
    virtual void BeginPlay() override;

private:
    TWeakObjectPtr<ACharacter> OwnerCharacter;
    bool bIsRightMouseButtonPressed = false;

    void MoveByCharacter(const FInputActionValue& Value);
    void MoveByCamera(const FInputActionValue& Value);
    bool CheckController() const;
};