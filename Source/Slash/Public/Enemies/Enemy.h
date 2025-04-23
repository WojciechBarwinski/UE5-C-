#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Characters/BaseCharacter.h"
//#include "Interfaces/HitInterface.h"
#include "Characters/CharacterTypes.h"
#include "Enemy.generated.h"



UCLASS()
class SLASH_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AEnemy();
	virtual void Tick(float DeltaTime) override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;

	UPROPERTY()
	AActor* CombatTarget;

	UPROPERTY(EditAnywhere)
	double CombatRadius = 500.f;

	UPROPERTY(EditAnywhere)
	double AttackRadius = 150.f;

	UPROPERTY(EditAnywhere)
	double PatrolRadius = 200.f;

	UPROPERTY()
	class AAIController* EnemyController;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray<AActor*> PatrolTargets;

	FTimerHandle PatrolTimer;

	void PatrolTimerFinished();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	AActor* CurrentTarget = nullptr;

	EEnemyState EnemyState = EEnemyState::EES_Patrolling;
protected:

	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);

	virtual void BeginPlay() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Die() override;
	
	bool InTargetRange(AActor* Target, double Radius);
	void MoveToTarget(AActor* Target);
	
	AActor* ChoosePatrolTarget();
	void CheckPatrolTarget();
	void CheckCombatTarget();
	virtual void Destroyed() override;
private:
	UPROPERTY()
	AActor* CurrentPatrolTarget = nullptr;

	int32 CurrentPatrolIndex = 0;
	AActor* LastPatrolTarget = nullptr;

	UPROPERTY(VisibleAnywhere)
	class UPawnSensingComponent* PawnSensing;

	UPROPERTY(VisibleAnywhere)
	class UHealthBarComponent* HealthBarWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeapon> WeaponClass;
public:	
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
