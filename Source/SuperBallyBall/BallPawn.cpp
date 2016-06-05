// Fill out your copyright notice in the Description page of Project Settings.

#include "SuperBallyBall.h"
#include "BallPawn.h"
#include "LevelContainer.h"
#include "Pickup.h"
#include "Guard.h"

// Sets default values
ABallPawn::ABallPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// The root component will be a sphere that reacts to physics
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	RootComponent = SphereComponent;
	SphereComponent->InitSphereRadius(40.0f);
	SphereComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	SphereComponent->SetSimulatePhysics(true);

	// Create and position a mesh component so the ball can be seen
	SphereVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	SphereVisual->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	if (SphereVisualAsset.Succeeded())
	{
		SphereVisual->SetStaticMesh(SphereVisualAsset.Object);
		SphereVisual->SetRelativeLocation(FVector(0.0f, 0.0f, -40.0f));
		SphereVisual->SetWorldScale3D(FVector(0.8f));
	}

	// Use a spring arm to give the camera smooth, natural-feeling motion.
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraAttachmentArm"));
	SpringArm->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	SpringArm->RelativeRotation = FRotator(-30.0f, 0.0f, 0.0f);
	SpringArm->SetAbsolute(false, true, false);
	SpringArm->TargetArmLength = 500.0f;
	SpringArm->bEnableCameraLag = false;

	// Create a camera and attach to the spring arm
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("ActualCamera"));
	Camera->AttachToComponent(SpringArm, FAttachmentTransformRules::KeepRelativeTransform, USpringArmComponent::SocketName);

	// Take control of the default player
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// Set the factor used to affect the LevelContainer's rotation amount from input
	InputRotationFactor = 2.0f;

	// Set the factor used to knock back the pawn when overlapping a guard
	KnockBackFactor = 4.0f;

	// No pickups collected at the beginning (spawn time)
	bPickupCollected = false;
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
	HandleOverlappingActors();
}

// Called to bind functionality to input
void ABallPawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	InputComponent->BindAxis("ChangeRoll", this, &ABallPawn::ChangeRoll);
	InputComponent->BindAxis("ChangePitch", this, &ABallPawn::ChangePitch);
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
}

// Change the roll of the LevelContainer's rotation relative to the world
void ABallPawn::ChangeRoll(float AxisValue)
{
	if (LevelContainer && ABallPawn::Controller && AxisValue != 0.0f)
	{
		const FRotator Rotation = ABallPawn::Controller->GetControlRotation();
		const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		LevelContainer->AddActorWorldRotation(FQuat(Direction, FMath::DegreesToRadians(InputRotationFactor * AxisValue)));		
	}
}

// Change the pitch of the LevelContainer's rotation relative to the world
void ABallPawn::ChangePitch(float AxisValue)
{
	if (LevelContainer && ABallPawn::Controller && AxisValue != 0.0f)
	{
		const FRotator Rotation = ABallPawn::Controller->GetControlRotation();
		const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		LevelContainer->AddActorWorldRotation(FQuat(Direction, FMath::DegreesToRadians(InputRotationFactor * AxisValue)));
	}
}

// Handle overlapping of the SphereComponent with pickups and guards
void ABallPawn::HandleOverlappingActors()
{
	// Get all overlapping Actors and store them in an array
	TArray<AActor*> OverlappedActors;
	SphereComponent->GetOverlappingActors(OverlappedActors);

	for (int32 i = 0; i != OverlappedActors.Num(); ++i)
	{
		// Cast the Actor to APickup
		APickup* const CollectedPickup = Cast<APickup>(OverlappedActors[i]);
		// If the cast is successful and the pickup is valid and active 
		if (CollectedPickup && !CollectedPickup->IsPendingKill() && CollectedPickup->IsActive())
		{
			// Transfer the pickup's color to the ball pawn
			UMaterialInstanceDynamic* Material = SphereVisual->CreateDynamicMaterialInstance(0, SphereVisual->GetMaterial(0));
			if (Material && !bPickupCollected)
			{
				Color = CollectedPickup->GetColor();
				Material->SetVectorParameterValue(FName("Color"), Color);
				bPickupCollected = true;
			}
			else if (Material)
			{
				Color = (Color + CollectedPickup->GetColor()) / 2;
				Material->SetVectorParameterValue(FName("Color"), Color);
			}

			// Destroy and deactivate the pickup
			CollectedPickup->Destroy();
			CollectedPickup->SetActive(false);
		}
		else
		{
			// Cast the Actor to AGuard
			AGuard* const CollidedGuard = Cast<AGuard>(OverlappedActors[i]);
			// If the cast is successful 
			if (CollidedGuard)
			{
				// Knock the BallPawn back
				SphereVisual->SetPhysicsLinearVelocity(-KnockBackFactor * GetVelocity());
			}
		}
	}
}
