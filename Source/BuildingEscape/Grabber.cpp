// Trademark Nerds.co.uk

#include "Grabber.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

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
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	FVector PlayerLocation;
	FRotator PlayerRotation;

	//The following is a 'void getter' function. Never seen one before. Takes OUTPUT parameters instead of INPUT parameters
	APlayerController* ThisPlayerController = GetWorld()->GetFirstPlayerController();
	ThisPlayerController->GetPlayerViewPoint(PlayerLocation, PlayerRotation);
	/*UE_LOG(LogTemp, Warning, TEXT("Position: %s || Rotation: %s"), *PlayerLocation.ToString(), *PlayerRotation.ToString());*/

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
}

