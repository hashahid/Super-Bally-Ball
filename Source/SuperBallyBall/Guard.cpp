// Fill out your copyright notice in the Description page of Project Settings.

#include "SuperBallyBall.h"
#include "Guard.h"

// Sets default values
AGuard::AGuard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create and configure a mesh component so the guard can be seen and collected
	GuardVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	RootComponent = GuardVisual;
	GuardVisual->SetCollisionProfileName(TEXT("OverlapAll"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> GuardVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cylinder.Shape_Cylinder"));
	if (GuardVisualAsset.Succeeded())
	{
		GuardVisual->SetStaticMesh(GuardVisualAsset.Object);
	}
}

void AGuard::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UMaterialInstanceDynamic* Material = GuardVisual->CreateDynamicMaterialInstance(0, GuardVisual->GetMaterial(0));
	if (Material)
	{
		Material->SetVectorParameterValue(FName("Color"), FLinearColor::Red);
	}
}

// Called when the game starts or when spawned
void AGuard::BeginPlay()
{
	Super::BeginPlay();
}
