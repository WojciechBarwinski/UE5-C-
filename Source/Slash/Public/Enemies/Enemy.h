#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "Enemy.generated.h"



UCLASS()
class SLASH_API AEnemy : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	AEnemy();
	virtual void Tick(float DeltaTime) override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;

	UPROPERTY(EditAnywhere, Category = Sounds)
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = VisualEffects)
	UParticleSystem* HitParticles;

protected:
	virtual void BeginPlay() override;
	void PlayHitReactMontage(const FName& SectionName);

private:

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* HitReactMontage;

	UPROPERTY(VisibleAnywhere)
	class UAttributeComponent* Attributes;

	UPROPERTY(VisibleAnywhere)
	class UHealthBarComponent* HealthBarWidget;
public:	
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
