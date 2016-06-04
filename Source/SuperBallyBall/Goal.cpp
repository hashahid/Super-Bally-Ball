// Fill out your copyright notice in the Description page of Project Settings.

#include "SuperBallyBall.h"
#include "Goal.h"

// Sets default values
AGoal::AGoal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create and configure a mesh component so the goal can be seen
	GoalVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	RootComponent = GoalVisual;
	GoalVisual->SetCollisionProfileName(UCollisionProfile::BlockAllDynamic_ProfileName);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> GoalVisualAsset(TEXT("/Game/Meshes/Ring_Mesh.Ring_Mesh"));
	if (GoalVisualAsset.Succeeded())
	{
		GoalVisual->SetStaticMesh(GoalVisualAsset.Object);
	}
}

// Called after construction and before BeginPlay
void AGoal::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UMaterialInstanceDynamic* Material = GoalVisual->CreateDynamicMaterialInstance(0, GoalVisual->GetMaterial(0));
	if (Material)
	{
		Material->SetVectorParameterValue(FName("Color"), Color);
	}
}

// Called when the game starts or when spawned
void AGoal::BeginPlay()
{
	Super::BeginPlay();
}
