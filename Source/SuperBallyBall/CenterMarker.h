// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "CenterMarker.generated.h"

UCLASS()
class SUPERBALLYBALL_API ACenterMarker : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACenterMarker();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	/** Sphere component to represent the marker */
	class USphereComponent* MarkerRoot;
};
