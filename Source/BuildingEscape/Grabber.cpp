// Trademark Nerds.co.uk

#include "Grabber.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/InputComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Math/TwoVectors.h"

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

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	///If physics handle is holding onto something move it's position each tick
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(GetLineTracePoints().v2);
	}
}

//Binds input action to grabbing code
void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Input Component not found on %s"), *(GetOwner()->GetName()));
	}
	else 
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}

//Checks to see if Physics Handle exists
void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Physics Handle not found on object %s"), *(GetOwner()->GetName()));
	}
}

//Grabs object in front of player when button is pressed
void UGrabber::Grab()
{
	///LINE TRACE out to set distance and hit any actors with collision channel = 'Physics Body'
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();

	///Attach a physics handle to this actor that's been hit
	if(ActorHit)
	{
		if (!PhysicsHandle) { return; }
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(),
			ComponentToGrab->GetOwner()->GetActorRotation()
		);
	}
}

void UGrabber::Release()
{
	if (!PhysicsHandle) { return; }
	PhysicsHandle->ReleaseComponent();
}

FTwoVectors UGrabber::GetLineTracePoints()
{
	///Query parameters
	FVector PlayerLocation;
	FRotator PlayerRotation;
	APlayerController* ThisPlayerController = GetWorld()->GetFirstPlayerController();

	ThisPlayerController->GetPlayerViewPoint(
		OUT PlayerLocation,
		OUT PlayerRotation
	);

	return FTwoVectors(PlayerLocation, PlayerLocation + PlayerRotation.Vector() * Reach);
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	FHitResult Hit;
	FTwoVectors TracePoints = GetLineTracePoints();
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		TracePoints.v1,
		TracePoints.v2,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);
	return Hit;
}