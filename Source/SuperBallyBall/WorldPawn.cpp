// Fill out your copyright notice in the Description page of Project Settings.

#include "SuperBallyBall.h"
#include "WorldPawn.h"

// Sets default values
AWorldPawn::AWorldPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// The root component is a sphere that contains the level
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	RootComponent = SphereComponent;
	SphereComponent->InitSphereRadius(1000.0f);
}

// Called when the game starts or when spawned
void AWorldPawn::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AWorldPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AWorldPawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);
}
