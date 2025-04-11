#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Test/Openable.h"
#include "Chest.generated.h"

UCLASS()
class SLASH_API AChest : public AActor, public IOpenable
{
	GENERATED_BODY()
	
public:	
	AChest();
	virtual void Tick(float DeltaTime) override;

	virtual void Open_Implementation(USceneComponent* InParent) override;
	
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	bool isOpen = false;
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* ChestMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USkeletalMeshComponent* ChestSkeletalMesh;

	UPROPERTY(VisibleAnywhere)
	class USphereComponent* Sphere;
	
private:	
	
	UPROPERTY(EditAnywhere, Category = "Breakable Properties")
	TArray<TSubclassOf<class ATreasure>> TreasureClasses;

	void PlayOpenCloseMontage(const FName SectionName);
};
