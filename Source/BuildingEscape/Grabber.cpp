// Trademark Nerds.co.uk

#include "Grabber.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/InputComponent.h"

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

	FindPhysicsHandleComponent();
	SetupInputComponent();
}

//Binds input action to grabbing code
void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Input Component not found on %s"), *(GetOwner()->GetName()));
	}
}

//Attaches physics handle to this 
void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{
		///working fine
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Physics Handle not found on object %s"), *(GetOwner()->GetName()));
	}
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab pressed"));

	///LINE TRACE out to set distance and hit any actors which colission channel Physics Body
	GetFirstPhysicsBodyInReach();

	///Attach a physics handle to this actor

	//TODO Attach physics handle
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab released"));

	//TODO Release physics handle
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	///If physics handle is holding onto something
	///Move it's position each tick
	
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	FVector PlayerLocation;
	FRotator PlayerRotation;

	//The following is a 'void getter' function. Never seen one before. Takes OUTPUT parameters instead of INPUT parameters
	APlayerController* ThisPlayerController = GetWorld()->GetFirstPlayerController();
	ThisPlayerController->GetPlayerViewPoint(PlayerLocation, PlayerRotation);

	FVector LineTraceEnd = PlayerLocation + PlayerRotation.Vector() * Reach;

	DrawDebugLine(
		GetWorld(),
		PlayerLocation,
		LineTraceEnd,
		FColor(0, 0, 255),
		false,
		0.0f,
		0.0f,
		2.0f
	);

	FHitResult Hit;
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(
		Hit,
		PlayerLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	AActor* ActorHit = Hit.GetActor();

	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor being hit is: %s"), *(ActorHit->GetName()));
	}
	return FHitResult();
}

