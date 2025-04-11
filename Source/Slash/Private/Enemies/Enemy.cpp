#include "Enemies/Enemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Slash/DebugMacros.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AttributeComponent.h"
#include "Components/WidgetComponent.h"
#include "HUD/HealthBarComponent.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

    Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
    HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar"));
    HealthBarWidget->SetupAttachment(GetRootComponent());
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
    if (HealthBarWidget)
    {
        HealthBarWidget->SetVisibility(false);
    }
}

void AEnemy::PlayHitReactMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
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

    if (CombatTarget)
    {
        const double DistanceToTarget = (CombatTarget->GetActorLocation() - GetActorLocation()).Size();
        if (DistanceToTarget > CombatRadius)
        {
            CombatTarget = nullptr;
            if (HealthBarWidget)
            {
                HealthBarWidget->SetVisibility(false);
            }
        }
    }
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

//void AEnemy::GetHit(const FVector& ImpactPoint)
//{
//    const FVector ImpactLocal = GetActorTransform().InverseTransformPosition(ImpactPoint);
//
//    // Usuwamy wartoœci ujemne - interesuje nas tylko wielkoœæ wzglêdna
//    const float AbsX = FMath::Abs(ImpactLocal.X);
//    const float AbsY = FMath::Abs(ImpactLocal.Y);
//
//    FString DirectionStr;
//    FName MontageDirection;
//
//    // Okreœlanie dominuj¹cego kierunku
//    if (AbsY >= AbsX) // Dominuje Y (przód/ty³)
//    {
//        if (ImpactLocal.Y > 0)
//        {
//            DirectionStr = "PRZODU";
//            MontageDirection = "FromFront";
//        }
//        else
//        {
//            DirectionStr = "TY£U";
//            MontageDirection = "FromBack";
//        }
//    }
//    else // Dominuje X (lewo/prawo)
//    {
//        if (ImpactLocal.X > 0)
//        {
//            DirectionStr = "PRAWEJ";
//            MontageDirection = "FromRight";
//        }
//        else
//        {
//            DirectionStr = "LEWEJ";
//            MontageDirection = "FromLeft";
//        }
//    }
//
//    // Logi z dodatkowymi informacjami
//    UE_LOG(LogTemp, Warning, TEXT("======================"));
//    UE_LOG(LogTemp, Warning, TEXT("Kierunek: %s"), *DirectionStr);
//    UE_LOG(LogTemp, Warning, TEXT("Wspó³rzêdne: X=%.1f | Y=%.1f"), ImpactLocal.X, ImpactLocal.Y);
//    UE_LOG(LogTemp, Warning, TEXT("Stosunek X/Y: %.2f"), AbsX / AbsY);
//    UE_LOG(LogTemp, Warning, TEXT("======================"));
//
//    PlayHitReactMontage(MontageDirection);
//}


void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

