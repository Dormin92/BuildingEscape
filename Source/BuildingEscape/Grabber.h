// Trademark Nerds.co.uk

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/InputComponent.h"
#include "Grabber.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere)
	float Reach = 100.0f;

	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputComponent = nullptr;

	//ray-cast and grab whatever is in reach
	void Grab();

	//Release the grbbed object
	void Release();

	//Get the first actor hit within a certain distance
	const FHitResult GetFirstPhysicsBodyInReach();

	void SetupInputComponent();

	void FindPhysicsHandleComponent();

	//Returns two vectors, the first is the player location, and the second is a point a set distance in front of the player viewpoint
	FTwoVectors GetLineTracePoints();
};
