// Fill out your copyright notice in the Description page of Project Settings.

#include "SuperBallyBall.h"
#include "BallPawn.h"
#include "LevelContainer.h"
#include "Pickup.h"

// Sets default values
ABallPawn::ABallPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// The root component will be a sphere that reacts to physics
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	RootComponent = SphereComponent;
	SphereComponent->InitSphereRadius(40.0f);
	SphereComponent->SetCollisionProfileName(TEXT("Pawn"));
	SphereComponent->SetSimulatePhysics(true);

	// Create and position a mesh component so the ball can be seen
	SphereVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	SphereVisual->AttachTo(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	if (SphereVisualAsset.Succeeded())
	{
		SphereVisual->SetStaticMesh(SphereVisualAsset.Object);
		SphereVisual->SetRelativeLocation(FVector(0.0f, 0.0f, -40.0f));
		SphereVisual->SetWorldScale3D(FVector(0.8f));
	}

	// Use a spring arm to give the camera smooth, natural-feeling motion.
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraAttachmentArm"));
	SpringArm->AttachTo(RootComponent);
	SpringArm->RelativeRotation = FRotator(-30.0f, 0.0f, 0.0f);
	SpringArm->SetAbsolute(false, true, false);
	SpringArm->TargetArmLength = 500.0f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 3.0f;

	// Create a camera and attach to the spring arm
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("ActualCamera"));
	Camera->AttachTo(SpringArm, USpringArmComponent::SocketName);

	// Take control of the default player
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void ABallPawn::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABallPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Handle pickups the ball came into contact with
	CollectPickup();

	// Restart the level if the ball falls below it
	if (LevelContainer && GetActorLocation().Z < LevelContainer->GetActorLocation().Z - LevelContainer->GetSphereComponent()->GetScaledSphereRadius())
	{
		UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
	}
}

// Called to bind functionality to input
void ABallPawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	InputComponent->BindAxis("ChangeRoll", this, &ABallPawn::ChangeRoll);
	InputComponent->BindAxis("ChangePitch", this, &ABallPawn::ChangePitch);
	InputComponent->BindAxis("ChangeYaw", this, &ABallPawn::ChangeYaw);
}

void ABallPawn::ChangeRoll(float AxisValue)
{
	if (LevelContainer)
	{
		LevelContainer->AddActorWorldRotation(FRotator(0.0f, 0.0f, AxisValue).Quaternion());
	}
}

void ABallPawn::ChangePitch(float AxisValue)
{
	if (LevelContainer)
	{
		LevelContainer->AddActorWorldRotation(FRotator(AxisValue, 0.0f, 0.0f).Quaternion());
	}
}

void ABallPawn::ChangeYaw(float AxisValue)
{
	if (LevelContainer)
	{
		LevelContainer->AddActorWorldRotation(FRotator(0.0f, AxisValue, 0.0f).Quaternion());
	}
}

void ABallPawn::CollectPickup()
{
	// Get all overlapping Actors and store them in an array
	TArray<AActor*> CollectedActors;
	SphereComponent->GetOverlappingActors(CollectedActors);

	for (int32 i = 0; i != CollectedActors.Num(); ++i)
	{
		// Cast the Actor to APickup
		APickup* const CollectedPickup = Cast<APickup>(CollectedActors[i]);
		// If the cast is successful and the pickup is valid and active 
		if (CollectedPickup && !CollectedPickup->IsPendingKill() && CollectedPickup->IsActive())
		{
			// Call the pickup's WasCollected function and deactivate it
			CollectedPickup->WasCollected();
			CollectedPickup->SetActive(false);
		}
	}
}
