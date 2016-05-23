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

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Return the sphere component
	FORCEINLINE class USphereComponent* GetSphereComponent() const { return SphereComponent; }

private:
	/** The root component, which is represented by a sphere */
	UPROPERTY(EditAnywhere)
	class USphereComponent* SphereComponent;
};
