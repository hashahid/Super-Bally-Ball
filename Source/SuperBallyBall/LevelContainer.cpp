// Fill out your copyright notice in the Description page of Project Settings.

#include "SuperBallyBall.h"
#include "LevelContainer.h"

// Sets default values
ALevelContainer::ALevelContainer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// The root component is a sphere that contains the level
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	RootComponent = SphereComponent;
	SphereComponent->InitSphereRadius(1000.0f);
}

// Called when the game starts or when spawned
void ALevelContainer::BeginPlay()
{
	Super::BeginPlay();
}
