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
	SphereComponent->SetLinearDamping(0.6f);
	SphereComponent->SetAngularDamping(0.6f);

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

	// No pickups collected at the beginning (spawn time)
	bPickupCollected = false;
}

// Called when the game starts or when spawned
void ABallPawn::BeginPlay()
{
	Super::BeginPlay();

	// Capture the pawn's inital location so it can be reset after falling
	StartingLocation = GetActorLocation();
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

	InputComponent->BindAction("ResetPawnAndLevel", IE_Pressed, this, &ABallPawn::ResetPawnAndLevel);

	InputComponent->BindAxis("ChangeRoll", this, &ABallPawn::ChangeRoll);
	InputComponent->BindAxis("ChangePitch", this, &ABallPawn::ChangePitch);
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
}

void ABallPawn::ResetPawnAndLevel()
{
	if (LevelContainer)
	{
		// Reset player, level, and camera positions
		UpdateLevelContainerLocation(FVector::ZeroVector);
		LevelContainer->SetActorRotation(FRotator::ZeroRotator);
		for (int32 i = 0; i != LevelContainer->GetChildrenActors().Num(); ++i)
		{
			LevelContainer->GetChildrenActors()[i]->SetActorLocation(LevelContainer->GetChildrenActorLocations()[i]);
		}
		TeleportTo(FVector(GetStartingLocation()), FRotator::ZeroRotator);
		GetSphereVisual()->SetPhysicsLinearVelocity(FVector::ZeroVector);
		GetSphereVisual()->SetPhysicsAngularVelocity(FVector::ZeroVector);
		GetController()->SetControlRotation(FRotator::ZeroRotator);
	}
}

// Change the roll of the LevelContainer's rotation relative to the world
void ABallPawn::ChangeRoll(float AxisValue)
{
	if (LevelContainer && ABallPawn::Controller && AxisValue != 0.0f)
	{
		UpdateLevelContainerLocation(GetActorLocation());

		const FRotator Rotation = ABallPawn::Controller->GetControlRotation();
		const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		LevelContainer->AddActorWorldRotation(FQuat(Direction, FMath::DegreesToRadians(LevelContainer->GetRotationFactor() * AxisValue)));		
	}
}

// Change the pitch of the LevelContainer's rotation relative to the world
void ABallPawn::ChangePitch(float AxisValue)
{
	if (LevelContainer && ABallPawn::Controller && AxisValue != 0.0f)
	{
		UpdateLevelContainerLocation(GetActorLocation());

		const FRotator Rotation = ABallPawn::Controller->GetControlRotation();
		const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		LevelContainer->AddActorWorldRotation(FQuat(Direction, FMath::DegreesToRadians(LevelContainer->GetRotationFactor() * AxisValue)));
	}
}

// Update the LevelContainer's location without updating the locations of its child actors
void ABallPawn::UpdateLevelContainerLocation(FVector NewLocation)
{
	for (AActor* ChildActor : LevelContainer->GetChildrenActors())
	{
		ChildActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	}

	LevelContainer->SetActorLocation(NewLocation);

	for (AActor* ChildActor : LevelContainer->GetChildrenActors())
	{
		ChildActor->AttachToActor(LevelContainer, FAttachmentTransformRules::KeepWorldTransform);
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
		if (CollectedPickup && CollectedPickup->IsActive())
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

			// Hide and deactivate the pickup
			CollectedPickup->PlayAudio();
			CollectedPickup->SetActorHiddenInGame(true);
			CollectedPickup->SetActive(false);
		}
		else
		{
			// Cast the Actor to AGuard
			AGuard* const CollidedGuard = Cast<AGuard>(OverlappedActors[i]);
			// If the cast is successful 
			if (CollidedGuard)
			{
				CollidedGuard->PlayAudio();
				// Knock the BallPawn back
				SphereVisual->SetPhysicsLinearVelocity(-1.0f * CollidedGuard->GetKnockBackFactor() * GetVelocity());
			}
		}
	}
}
