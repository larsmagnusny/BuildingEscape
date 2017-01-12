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
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandleComponent();
	SetupInputComponent();
}

void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("Error: No UPhysicsHandleComponent detected in Actor: %s"), *GetOwner()->GetName());
	}
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (!InputComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Error: No UInputComponent detected in Actor: %s"), *GetOwner()->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UInputComponent found in Actor: %s"), *GetOwner()->GetName());
		// Bind the input axis
		InputComponent->BindAction(FName("Grab"), EInputEvent::IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction(FName("Grab"), EInputEvent::IE_Released, this, &UGrabber::Release);
	}
}


void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab Pressed!"));

	// LINE TRACE and see if we reach any actors with physics body collision set
	FHitResult Hit = GetFirstPhysicsBodyInReach();

	// If we hit something then attach a physics handle
	// TODO attach physics handle
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab Released!"));
	// TODO release physics handle
}

// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
}


const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	FVector Location = FVector(0.0f, 0.0f, 0.0f);
	FRotator Rotation = FRotator(0.0f, 0.0f, 0.0f);
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT Location, OUT Rotation);

	FVector LineTraceEnd = Location + Rotation.Vector()*Reach;

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

	return Hit;
}