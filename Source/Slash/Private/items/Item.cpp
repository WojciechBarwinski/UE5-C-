// Fill out your copyright notice in the Description page of Project Settings.


#include "items/Item.h"
#include "Slash/DebugMacros.h"


// Sets default values
AItem::AItem()
{
 	
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	RootComponent = ItemMesh;
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
}

float AItem::TransformedSin()
{
	return Amplitude * FMath::Sin(RunningTime * TimeConstant);
}

float AItem::TransformedCos()
{
	return Amplitude * FMath::Cos(RunningTime * TimeConstant);
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RunningTime += DeltaTime;

	//float DeltaZ = Amplitude * FMath::Sin(RunningTime * TimeConstant);

	//AddActorWorldOffset(FVector(0.f, 0.f, DeltaZ));
	DRAW_SPHERE_SingleFrame(GetActorLocation());
	DRAW_VECTOR_SingleFrame(GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 100.f);
}

//// Movement rate in units of cm/s
//float MovementRate = 50.f;
//float RotationRate = 45.f;
//
//// MovementRate * DeltaTime (cm/s) * (s/frame) = (cm/frame)
//AddActorWorldOffset(FVector(MovementRate* DeltaTime, 0.f, 0.f));
//AddActorWorldRotation(FRotator(0.f, RotationRate* DeltaTime, 0.f));