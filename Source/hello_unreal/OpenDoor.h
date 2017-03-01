// copy write chinmay

#pragma once

#include "Components/ActorComponent.h"
#include "OpenDoor.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOpenRequest);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HELLO_UNREAL_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

	// Called when the game starts
	virtual void BeginPlay() override;

	void DoorOpen();
	void DoorClose();

	UPROPERTY(BlueprintAssignable)
	FOnOpenRequest OnOpenRequest;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

private:
	UPROPERTY(VisibleAnyWhere)
	float OpenAngle = 90.0f;
	
	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate;

	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 1.f;

	float LastDoorOpenDoor;

	float GetTotalMassOfActorOnPlate();

	AActor* ActorThatOpens;
	AActor* Owner;

};
