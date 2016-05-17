// Fill out your copyright notice in the Description page of Project Settings.

#include "SuperBallyBall.h"
#include "BallPawn.h"
#include "WorldPawn.h"
#include "BallPawnMovementComponent.h"

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
	SpringArm->RelativeRotation = FRotator(-45.f, 0.f, 0.f);
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 3.0f;

	// Create a camera and attach to the spring arm
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("ActualCamera"));
	Camera->AttachTo(SpringArm, USpringArmComponent::SocketName);

	// Take control of the default player
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// Create an instance of the movement component, and tell it to update the root.
	MovementComponent = CreateDefaultSubobject<UBallPawnMovementComponent>(TEXT("CustomMovementComponent"));
	MovementComponent->UpdatedComponent = RootComponent;
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
}

// Called to bind functionality to input
void ABallPawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	InputComponent->BindAxis("ChangeRoll", this, &ABallPawn::ChangeRoll);
	InputComponent->BindAxis("ChangePitch", this, &ABallPawn::ChangePitch);
	InputComponent->BindAxis("ChangeYaw", this, &ABallPawn::ChangeYaw);
}

UPawnMovementComponent* ABallPawn::GetMovementComponent() const
{
	return MovementComponent;
}

void ABallPawn::ChangeRoll(float AxisValue)
{
	FTransform Transform = WorldPawn->GetTransform();
	Transform.ConcatenateRotation(FRotator(0.0f, 0.0f, AxisValue).Quaternion());
	Transform.NormalizeRotation();
	WorldPawn->SetActorTransform(Transform);
}

void ABallPawn::ChangePitch(float AxisValue)
{
	FTransform Transform = WorldPawn->GetTransform();
	Transform.ConcatenateRotation(FRotator(AxisValue, 0.0f, 0.0f).Quaternion());
	Transform.NormalizeRotation();
	WorldPawn->SetActorTransform(Transform);
}

void ABallPawn::ChangeYaw(float AxisValue)
{
	FTransform Transform = WorldPawn->GetTransform();
	Transform.ConcatenateRotation(FRotator(0.0f, AxisValue, 0.0f).Quaternion());
	Transform.NormalizeRotation();
	WorldPawn->SetActorTransform(Transform);
}
