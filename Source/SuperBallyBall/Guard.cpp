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

	// Create and configure the audio component so the guard can play a sound when it is overlapped
	GuardAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("GuardAudio"));
	GuardAudio->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	GuardAudio->bAutoActivate = false;

	// Set the factor used to knock back the pawn when overlapping a guard
	KnockBackFactor = 1.0f;
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

// Play the audio component's referenced sound
void AGuard::PlayAudio()
{
	GuardAudio->Activate(true);
	GuardAudio->Play();
}
