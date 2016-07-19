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

	/** Reset the pawn, level, and camera's orientations to their starting values */
	void ResetPawnAndLevel();

	/** Return the pawn's static mesh */
	FORCEINLINE class UStaticMeshComponent* GetSphereVisual() const { return SphereVisual; }

	/** Return the level container */
	FORCEINLINE class ALevelContainer* GetLevelContainer() const { return LevelContainer; }

	/** Return the starting location of the pawn */
	FORCEINLINE FVector GetStartingLocation() const { return StartingLocation; }

	/** Return the color of this pawn's mesh's material */
	FORCEINLINE FLinearColor GetColor() const { return Color; }

private:
	/** Change the roll of the LevelContainer's rotation relative to the world */
	void ChangeRoll(float AxisValue);

	/** Change the pitch of the LevelContainer's rotation relative to the world */
	void ChangePitch(float AxisValue);

	/** Update the LevelContainer's location without updating the locations of its child actors */
	void UpdateLevelContainerLocation(FVector NewLocation);

	/** Handle overlapping of the SphereComponent with pickups and guards */
	void HandleOverlappingActors();

private:
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

	/** The pawn's starting location. Used when resetting the level after falling outside level's bounds */
	FVector StartingLocation;

	/** The color of this pawn's mesh's material. Matching the goal's color while passing through it wins the game */
	FLinearColor Color;

	/** Used to SET ball's color on initial pickup overlap and AVERAGE it thereafter */
	bool bPickupCollected;
};
