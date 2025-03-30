#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Characters/CharacterTypes.h"
#include "Attachable.generated.h"

UINTERFACE(MinimalAPI)
class UAttachable : public UInterface
{
	GENERATED_BODY()
};

class SLASH_API IAttachable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Attachment")
	void Attach(USceneComponent* InParent);

	UFUNCTION(BlueprintNativeEvent, Category = "Attachment")
	ECharacterState GetCharacterState() const; 

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Attachment")
	void SheathedWeapon(USceneComponent* InParent);
};
