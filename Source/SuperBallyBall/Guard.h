// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Guard.generated.h"

UCLASS()
class SUPERBALLYBALL_API AGuard : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGuard();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	/** Return the knock back factor */
	FORCEINLINE float GetKnockBackFactor() const { return KnockBackFactor; }

	/** Play the sound referenced to by the guard's audio component */
	void PlayAudio();

private:
	/** Static mesh to represent the pickup */
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* GuardVisual;

	/** The sound to play when the guard is overlapped by the BallPawn */
	UPROPERTY(EditAnywhere)
	class UAudioComponent* GuardAudio;

	/** Magnitude of factor used to knock back the ball pawn when it collides with a guard */
	UPROPERTY(EditAnywhere)
	float KnockBackFactor;
};
