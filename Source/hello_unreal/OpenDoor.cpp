// copy write chinmay

#include "hello_unreal.h"
#include "OpenDoor.h"


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
	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
	Owner = GetOwner();
	LastDoorOpenDoor = 0;
	// ...
	
}

void UOpenDoor::DoorOpen()
{
	//Owner->SetActorRotation(FRotator(0.0f, 90, 0));
	UE_LOG(LogTemp, Warning, TEXT("I Actor is %s"), *Owner->GetName());

	OnOpenRequest.Broadcast();
}

void UOpenDoor::DoorClose()
{
	GetOwner()->SetActorRotation(FRotator(0.0f, 0, 0));
	UE_LOG(LogTemp, Warning, TEXT("I Actor is %s close"), *Owner->GetName());
}

float UOpenDoor::GetTotalMassOfActorOnPlate()
{
	float TotalMass = 0.f;
	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OverlappingActors);

	for ( auto &i: OverlappingActors)
	{
		TotalMass += i->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("Door open for %s"), *i->GetName());
	}

	return TotalMass;
}

// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
	//Poll the Trigger Volune
	
	if (GetTotalMassOfActorOnPlate() >= 150.f)
	{
		//if thr Actor is in the volume open it
		DoorOpen();
		LastDoorOpenDoor = GetWorld()->GetTimeSeconds();
	}
	//Checking if the time changes if the game is paused-->answer is no
	//UE_LOG(LogTemp, Warning, TEXT("The current world time %s"), *(FString::SanitizeFloat(GetWorld()->GetTimeSeconds())));
	if (LastDoorOpenDoor && GetWorld()->GetTimeSeconds() - LastDoorOpenDoor >= DoorCloseDelay)
	{
		DoorClose();
		LastDoorOpenDoor = 0;
	}
	
	// ... 
}

