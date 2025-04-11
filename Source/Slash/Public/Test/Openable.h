#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Openable.generated.h"

UINTERFACE(MinimalAPI)
class UOpenable : public UInterface
{
	GENERATED_BODY()
};

class SLASH_API IOpenable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void Open(USceneComponent* InParent);
};
