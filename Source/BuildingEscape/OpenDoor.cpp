// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "Gameframework/Actor.h"
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
	ActorOpeningDoor = GetWorld()->GetFirstPlayerController()->GetPawn();
	Owner = GetOwner();
	
	if (PressurePlate == nullptr) {
		UE_LOG(LogTemp,Error,TEXT("Missing Pressure Plate"))
	}
	

	
}





// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (GetTotalActorsMassOnPlate()>PlateActivitionMass) {
		OnOpen.Broadcast();
	}
	else {
		OnClose.Broadcast();
	}

	

	// ...
}


float UOpenDoor::GetTotalActorsMassOnPlate() {
	float TotalMass = 0;
	//Get all the overlapping actors
	TArray<AActor*> OverlappingActors;
	if (PressurePlate == nullptr) { return TotalMass; }
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	//Iterate over all actors on plate and adding their masses
	for (const auto *Actor : OverlappingActors) {
		TotalMass = TotalMass + Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	
	return TotalMass;
}

