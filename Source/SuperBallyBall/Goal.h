// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Goal.generated.h"

UCLASS()
class SUPERBALLYBALL_API AGoal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGoal();

	// Called after construction and before BeginPlay
	virtual void PostInitializeComponents() override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Return the color of the goal's mesh's material */
	FORCEINLINE FLinearColor GetColor() const { return Color; }

	/** Play the sound referenced to by the goal's audio component */
	void PlayAudio();

private:
	/** Static mesh to represent the goal */
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* GoalVisual;

	/** The color of the goal's mesh's material. Matching the BallPawn's color when it passes through wins the game */
	UPROPERTY(EditAnywhere)
	FLinearColor Color;

	/** The sound to play when the goal is crossed by the BallPawn and their colors match */
	UPROPERTY(EditAnywhere)
	class UAudioComponent* GoalAudio;
};
