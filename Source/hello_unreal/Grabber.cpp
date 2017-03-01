// copy write chinmay

#include "hello_unreal.h"
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
	UE_LOG(LogTemp, Warning, TEXT("Grabber on way"));

	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (PhysicsHandle)
	{
		//Physics Handle is found
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing physics handle component"),*GetOwner()->GetName());
	}

	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (InputComponent)
	{
		//Input Component is found
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber :: Grab);
		InputComponent->BindAction("Release", IE_Released , this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing input component"), *GetOwner()->GetName());
	}
}

void UGrabber::Grab() {
	UE_LOG(LogTemp, Warning, TEXT("text grabbed pressed"));

	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();
	
	if (ActorHit)
	{
		PhysicsHandle->GrabComponent(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true
		);
	}
	
}

void UGrabber::Release() {
	UE_LOG(LogTemp, Warning, TEXT("text released pressed"));
	PhysicsHandle->ReleaseComponent();
}
// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	/*UE_LOG(LogTemp, Warning,TEXT("%s ,%s"), *PlayerViewPointLocation.ToString(),*PlayerViewPointRotation.ToString() );*/

	//Draw a red trace in the world.

	FVector LineTracerEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * 100;
	// ...
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(LineTracerEnd);
	}
}

const FHitResult UGrabber ::GetFirstPhysicsBodyInReach(){
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	/*UE_LOG(LogTemp, Warning,TEXT("%s ,%s"), *PlayerViewPointLocation.ToString(),*PlayerViewPointRotation.ToString() );*/

	//Draw a red trace in the world.

	FVector LineTracerEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * 100;

	DrawDebugLine(
		GetWorld(),
		PlayerViewPointLocation,
		LineTracerEnd,
		FColor(255, 0, 0),
		false,
		0.0f,
		0.0f,
		10.0f
	);

	///Line-trace (AKA ray-cast) out to reach distance.
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTracerEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	AActor *isActor = Hit.GetActor();
	if (isActor)
		UE_LOG(LogTemp, Warning, TEXT("I hit %s"), *isActor->GetName());

	return Hit;
}