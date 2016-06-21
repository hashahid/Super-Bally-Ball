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
	SphereComponent->InitSphereRadius(2500.0f);
}

// Called after construction and before BeginPlay
void ALevelContainer::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	TArray<USceneComponent*> ChildrenComponents;
	RootComponent->GetChildrenComponents(false, ChildrenComponents);
	for (USceneComponent* ChildComponent : ChildrenComponents)
	{
		ChildrenActors.Add(ChildComponent->GetOwner());
		ChildrenActorLocations.Add(ChildComponent->GetOwner()->GetActorLocation());
	}
}

// Called when the game starts or when spawned
void ALevelContainer::BeginPlay()
{
	Super::BeginPlay();
}
