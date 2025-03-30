#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterAttackHandler.generated.h"

class ASlashCharacter;
struct FInputActionValue;

UCLASS()//( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SLASH_API UCharacterAttackHandler : public UActorComponent
{
	GENERATED_BODY()

public:	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* AttackMontage;

	void Attack(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void AttackEnd();

protected:
	virtual void BeginPlay() override;
	
private:	
	TWeakObjectPtr<ACharacter> OwnerCharacter;
	TWeakObjectPtr<ASlashCharacter> SlashCharacter;
	void PlayAttackMontage();
	bool CanAttack();
};
