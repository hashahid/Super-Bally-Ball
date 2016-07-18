// Fill out your copyright notice in the Description page of Project Settings.

#include "SuperBallyBall.h"
#include "Pickup.h"

// Sets default values
APickup::APickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// All pickups start active
	bIsActive = true;

	// Create and configure a mesh component so the pickup can be seen and collected
	PickupVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	RootComponent = PickupVisual;
	PickupVisual->SetCollisionProfileName(TEXT("OverlapAll"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> PickupVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Trim.Shape_Trim"));
	if (PickupVisualAsset.Succeeded())
	{
		PickupVisual->SetStaticMesh(PickupVisualAsset.Object);
	}

	// Create and configure the audio component so the pickup can play a sound when it is overlapped
	PickupAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("PickupAudio"));
	PickupAudio->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	PickupAudio->bAutoActivate = false;
}

// Called after construction and before BeginPlay
void APickup::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UMaterialInstanceDynamic* Material = PickupVisual->CreateDynamicMaterialInstance(0, PickupVisual->GetMaterial(0));
	if (Material)
	{
		Material->SetVectorParameterValue(FName("Color"), Color);
	}
}

// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorLocalRotation(FRotator(0.0f, DeltaTime * 50.0f, 0.0f).Quaternion());
}

// Changes active state
void APickup::SetActive(bool NewPickupState)
{
	bIsActive = NewPickupState;
}

// Play the audio component's referenced sound
void APickup::PlayAudio()
{
	PickupAudio->Activate(true);
	PickupAudio->Play();
}
