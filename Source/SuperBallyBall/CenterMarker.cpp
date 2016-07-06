// Fill out your copyright notice in the Description page of Project Settings.

#include "SuperBallyBall.h"
#include "CenterMarker.h"

// Sets default values
ACenterMarker::ACenterMarker()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// The root component will be a simple sphere
	MarkerRoot = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	RootComponent = MarkerRoot;
	MarkerRoot->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
}

// Called when the game starts or when spawned
void ACenterMarker::BeginPlay()
{
	Super::BeginPlay();
}
