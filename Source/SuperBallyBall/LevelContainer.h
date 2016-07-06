// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "LevelContainer.generated.h"

UCLASS()
class SUPERBALLYBALL_API ALevelContainer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelContainer();

	// Called after construction and before BeginPlay
	virtual void PostInitializeComponents() override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Return the sphere component */
	FORCEINLINE class USphereComponent* GetSphereComponent() const { return SphereComponent; }

	/** Return the child actors */
	FORCEINLINE TArray<AActor*> GetChildrenActors() const { return ChildrenActors; }

	/** Return the child actors' locations */
	FORCEINLINE TArray<FVector> GetChildrenActorLocations() const { return ChildrenActorLocations; }

	/** Return the rotation factor */
	FORCEINLINE float GetRotationFactor() const { return RotationFactor; }

private:
	/** The root component, which is represented by a sphere */
	UPROPERTY(EditAnywhere)
	class USphereComponent* SphereComponent;

	/** All actors (therefore the physical level) attached to the LevelContainer */
	TArray<AActor*> ChildrenActors;

	/** The inital locations of the LevelContainer's child actors. Used for resetting the level */
	TArray<FVector> ChildrenActorLocations;

	/** The factor with which to multiply the AxisValue from input (i.e. the rotation amount) */
	UPROPERTY(EditAnywhere)
	float RotationFactor;
};
