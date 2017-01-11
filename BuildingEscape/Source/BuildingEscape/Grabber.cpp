// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "Grabber.h"


#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	// ...
	UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty!"));

	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("Error: No UPhysicsHandleComponent detected in Actor: %s"), *GetOwner()->GetName());
	}
}


// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );


	// Get the player viewpoint this tick
	FVector Location = FVector(0.0f, 0.0f, 0.0f);
	FRotator Rotation = FRotator(0.0f, 0.0f, 0.0f);
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT Location, OUT Rotation);

	/*UE_LOG(LogTemp, Warning, TEXT("Player location: %s, rotation: %s"), *Location.ToString(), *Rotation.ToString());
	*/

	

	FVector LineTraceEnd = Location + Rotation.Vector()*Reach;

	// Draw a red trace in the world to visualize
	DrawDebugLine(
		GetWorld(),
		Location,
		LineTraceEnd,
		FColor(255, 0, 0),
		false,
		0.f,
		0,
		1.0f
	);

	// Setup Query Parameters...
	FCollisionQueryParams QueryParameters = FCollisionQueryParams(FName(TEXT("")), false, GetOwner());

	// Line-trace ( AKA Ray-Cast ) out to reach distance
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		Location,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		QueryParameters
	);
	// See what we hit
	AActor* ObjectHit = Hit.GetActor();

	if (ObjectHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Line Trace hit: %s"), *ObjectHit->GetName())
		
	}
}

