#include "Enemies/Enemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Slash/DebugMacros.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AttributeComponent.h"
#include "Components/WidgetComponent.h"
#include "Items/Weapons/Weapon.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HUD/HealthBarComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "AIController.h"
#include "Perception/PawnSensingComponent.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

    
    HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar"));
    HealthBarWidget->SetupAttachment(GetRootComponent());

    GetCharacterMovement()->bOrientRotationToMovement = true;
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
    PawnSensing->SightRadius = 4000.f;
    PawnSensing->SetPeripheralVisionAngle(45.f);
}

void AEnemy::Attack()
{
    Super::Attack();
    PlayAttackMontage();
}

void AEnemy::PlayAttackMontage()
{
    Super::PlayAttackMontage();

    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    if (AnimInstance && AttackMontage)
    {
        AnimInstance->Montage_Play(AttackMontage);
        const int32 Selection = FMath::RandRange(0, 2);
        FName SectionName = FName();
        switch (Selection)
        {
        case 0:
            SectionName = FName("Attack1");
            break;
        case 1:
            SectionName = FName("Attack2");
            break;
        case 2:
            SectionName = FName("Attack3Root");
            break;
        default:
            break;
        }
        AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
    }
}

void AEnemy::PawnSeen(APawn* SeenPawn)
{
    if (EnemyState == EEnemyState::EES_Attacking || EnemyState == EEnemyState::EES_Chasing) return;
    if (SeenPawn->ActorHasTag(FName("SlashCharacter")))
    {
        EnemyState = EEnemyState::EES_Chasing;
        GetWorldTimerManager().ClearTimer(PatrolTimer);
        GetCharacterMovement()->MaxWalkSpeed = 300.f;
        CombatTarget = SeenPawn;
        if (EnemyState != EEnemyState::EES_Attacking)
        {
            EnemyState = EEnemyState::EES_Chasing;
            MoveToTarget(CombatTarget);
        }
    }
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
    if (HealthBarWidget)
    {
        HealthBarWidget->SetVisibility(false);
    }

    EnemyController = Cast<AAIController>(GetController());

    if (PatrolTargets.Num() > 0)
    {
        CurrentPatrolTarget = PatrolTargets[0];
        LastPatrolTarget = CurrentPatrolTarget;
		CurrentTarget = CurrentPatrolTarget;
		MoveToTarget(CurrentPatrolTarget);
    }

    if (PawnSensing)
    {
        PawnSensing->OnSeePawn.AddDynamic(this, &AEnemy::PawnSeen);
    }

    UWorld* World = GetWorld();
    if (World && WeaponClass)
    {
        AWeapon* DefaultWeapon = World->SpawnActor<AWeapon>(WeaponClass);
        DefaultWeapon->Execute_Attach(DefaultWeapon, this->GetMesh());
        EquippedWeapon = DefaultWeapon;
    }

}


float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    if (Attributes && HealthBarWidget)
    {
        Attributes->ReceiveDamage(DamageAmount);
        HealthBarWidget->SetHealthPercent(Attributes->GetHealthPercent());
    }
    CombatTarget = EventInstigator->GetPawn();
    EnemyState = EEnemyState::EES_Chasing;
    GetCharacterMovement()->MaxWalkSpeed = 300.f;
    MoveToTarget(CombatTarget);
    return DamageAmount;
}

void AEnemy::Die()
{
    if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
    {
        if (DeathMontage)
        {
            AnimInstance->Montage_Play(DeathMontage);
            const int32 RandomSectionIndex = FMath::RandRange(0, DeathMontage->CompositeSections.Num() - 1);
            AnimInstance->Montage_JumpToSection(DeathMontage->GetSectionName(RandomSectionIndex), DeathMontage);
        }
    }

    if (HealthBarWidget)
    {
        HealthBarWidget->SetVisibility(false);
    }
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    SetLifeSpan(3.f);
}

void AEnemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (EnemyState > EEnemyState::EES_Patrolling)
    {
        CheckCombatTarget();
    }
    else
    {
        CheckPatrolTarget();
    }
}

bool AEnemy::InTargetRange(AActor* Target, double Radius)
{
    if (Target == nullptr) return false;
    const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();
    return DistanceToTarget <= Radius;
}

void AEnemy::CheckPatrolTarget()
{
    if (InTargetRange(CurrentPatrolTarget, PatrolRadius)) {
    CurrentTarget = ChoosePatrolTarget();
    GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::PatrolTimerFinished, 5.f);
    }
}

void AEnemy::CheckCombatTarget()
{
    if (!InTargetRange(CombatTarget, CombatRadius))
    {
        // Outside combat radius, lose interest
        CombatTarget = nullptr;
        if (HealthBarWidget)
        {
            HealthBarWidget->SetVisibility(false);
        }
        EnemyState = EEnemyState::EES_Patrolling;
        GetCharacterMovement()->MaxWalkSpeed = 125.f;
        MoveToTarget(LastPatrolTarget);
    }

    else if (!InTargetRange(CombatTarget, AttackRadius) && EnemyState != EEnemyState::EES_Chasing)
    {
        // Outside attack range, chase character
        EnemyState = EEnemyState::EES_Chasing;
        GetCharacterMovement()->MaxWalkSpeed = 300.f;
        MoveToTarget(CombatTarget);
    }

    else if (InTargetRange(CombatTarget, AttackRadius) && EnemyState != EEnemyState::EES_Attacking)
    {
        // Inside attack range, attack character
        EnemyState = EEnemyState::EES_Attacking;
        Attack();
    }
}

void AEnemy::Destroyed()
{
    if (EquippedWeapon)
    {
        EquippedWeapon->Destroy();
    }
}

AActor* AEnemy::ChoosePatrolTarget()
{
        CurrentPatrolIndex = (CurrentPatrolIndex + 1) % PatrolTargets.Num();
        CurrentPatrolTarget = PatrolTargets[CurrentPatrolIndex];

        if (CurrentPatrolTarget != LastPatrolTarget)
        {
            LastPatrolTarget = CurrentPatrolTarget;
            return CurrentPatrolTarget;
        }
    return nullptr;
}

void AEnemy::PatrolTimerFinished()
{
	MoveToTarget(CurrentTarget);
}

void AEnemy::MoveToTarget(AActor* Target)
{
    if (EnemyController == nullptr || Target == nullptr) return;
    FAIMoveRequest MoveRequest;
    MoveRequest.SetGoalActor(Target);
    MoveRequest.SetAcceptanceRadius(50.f);
    EnemyController->MoveTo(MoveRequest);
}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
    const FVector Forward = GetActorForwardVector();
    // Lower Impact Point to the Enemy's Actor Location Z
    const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
    const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();

    // Forward * ToHit = |Forward||ToHit| * cos(theta)
    // |Forward| = 1, |ToHit| = 1, so Forward * ToHit = cos(theta)
    const double CosTheta = FVector::DotProduct(Forward, ToHit);
    // Take the inverse cosine (arc-cosine) of cos(theta) to get theta
    double Theta = FMath::Acos(CosTheta);
    // convert from radians to degrees
    Theta = FMath::RadiansToDegrees(Theta);

    // if CrossProduct points down, Theta should be negative
    const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
    if (CrossProduct.Z < 0)
    {
        Theta *= -1.f;
    }

    FName Section("FromBack");

    if (Theta >= -45.f && Theta < 45.f)
    {
        Section = FName("FromFront");
    }
    else if (Theta >= -135.f && Theta < -45.f)
    {
        Section = FName("FromLeft");
    }
    else if (Theta >= 45.f && Theta < 135.f)
    {
        Section = FName("FromRight");
    }
    
    if (HealthBarWidget)
    {
        HealthBarWidget->SetVisibility(true);
    }

    if (Attributes && Attributes->IsAlive())
    {
        PlayHitReactMontage(Section);
    }
    else
    {
        Die();
    }

    if (HitSound)
    {
        UGameplayStatics::PlaySoundAtLocation(
            this,
            HitSound,
            ImpactPoint
        );
    }

    if (HitParticles && GetWorld())
    {
        UGameplayStatics::SpawnEmitterAtLocation(
            GetWorld(),
            HitParticles,
            ImpactPoint
        );
    }
}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

