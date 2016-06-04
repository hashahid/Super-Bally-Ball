// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "BallPawn.generated.h"

UCLASS()
class SUPERBALLYBALL_API ABallPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABallPawn();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	/** Return the level container */
	FORCEINLINE class UStaticMeshComponent* GetSphereVisual() const { return SphereVisual; }

	/** Return the level container */
	FORCEINLINE class ALevelContainer* GetLevelContainer() const { return LevelContainer; }

private:
	/** Change the roll of the LevelContainer's rotation relative to the world */
	void ChangeRoll(float AxisValue);

	/** Change the pitch of the LevelContainer's rotation relative to the world */
	void ChangePitch(float AxisValue);

	/** Change the yaw of the LevelContainer's rotation relative to the world */
	void ChangeYaw(float AxisValue);

	/** Handle overlapping of the SphereComponent with pickups and guards */
	void HandleOverlappingActors();

	/** The root component, which is represented by a sphere */
	UPROPERTY(EditAnywhere)
	class USphereComponent* SphereComponent;

	/** The static mesh which will serve as the visual representation of the pawn */
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* SphereVisual;

	/** Container for the camera */
	UPROPERTY(EditAnywhere)
	class USpringArmComponent* SpringArm;

	/** The camera which will look down on the ball from behind */
	UPROPERTY(EditAnywhere)
	class UCameraComponent* Camera;

	/** Reference to the ALevelContainer whose rotation is controlled by the pawn */
	UPROPERTY(EditAnywhere)
	class ALevelContainer* LevelContainer;

	/** The factor with which to multiply the AxisValue from input */
	UPROPERTY(EditAnywhere)
	float InputRotationFactor;

	/** Magnitude of factor used to knock back the pawn when it collides with a guard */
	UPROPERTY(EditAnywhere)
	float KnockBackFactor;

	/** Used to SET ball's color on initial pickup overlap and AVERAGE it thereafter */
	bool bPickupCollected;
};
