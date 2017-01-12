// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "OpenDoor.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();
}

void UOpenDoor::OpenDoor()
{
	// Set the door rotation
	Owner->SetActorRotation(FRotator(0.0f, OpenAngle, 0.0f));
}

void UOpenDoor::CloseDoor()
{
	// Set the door rotation
	Owner->SetActorRotation(FRotator(0.0f, -90.0f, 0.0f));
}


// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...

	// Poll the triggervolume every Frame
	// If the ActorThatOpens is in the volume
	if (GetTotalMassOfActorsOnPlate() > 50.f)	// TODO make into a parameter
	{
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}

	// Check if it's time to close the door
	if (LastDoorOpenTime + DoorCloseDelay < GetWorld()->GetTimeSeconds() && LastDoorOpenTime != 0.0f)
	{
		CloseDoor();
		LastDoorOpenTime = 0.0f;
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.f;

	// Find all the overlapping actors
	TArray<AActor*> OverLappingActors;
	PressurePlate->GetOverlappingActors(OUT OverLappingActors);

	for (const auto& Actor : OverLappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	// Iterate through them adding their masses

	return TotalMass;
}