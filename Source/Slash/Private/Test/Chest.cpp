#include "Test/Chest.h"
#include "Components/SkeletalMeshComponent.h"  // Musisz do³¹czyæ ten nag³ówek!
#include "Components/SphereComponent.h"


AChest::AChest()
{
	PrimaryActorTick.bCanEverTick = false;

	ChestSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ChestSkeletalMesh"));
	RootComponent = ChestSkeletalMesh;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(GetRootComponent());
}

void AChest::BeginPlay()
{
	Super::BeginPlay();
    
}

void AChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AChest::Open_Implementation(USceneComponent* InParent)
{
	if (!isOpen)
	{ 
		UE_LOG(LogTemp, Warning, TEXT("otwiera skrzynie"));
		isOpen = true;
		return;
	}
	else if (isOpen)
	{
		UE_LOG(LogTemp, Warning, TEXT("zamyka skrzynie skrzynie"));
		isOpen = false;
		return;
	}
}

void AChest::PlayOpenCloseMontage(const FName SectionName)
{
	UAnimInstance* AnimInstance = ChestSkeletalMesh->GetAnimInstance();
	if (AnimInstance && ChestMontage) {
		UE_LOG(LogTemp, Warning, TEXT("play montage"));

		AnimInstance->Montage_Play(ChestMontage);
		AnimInstance->Montage_JumpToSection(SectionName, ChestMontage);
	}
}

