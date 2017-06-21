// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "Gameframework/Actor.h"
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
	FindPhysicsHandleComponent();
	MapInput();	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (Handle == nullptr) { return; }
	if (Handle->GrabbedComponent) {
		Handle->SetTargetLocation(PlayerLineTraceEnd());
	}

}
//Find the attached Physics Handler
void UGrabber::FindPhysicsHandleComponent() {
	Handle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (Handle == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("%s missing physics handle component"), *(GetOwner()->GetName()))
	}
}

//Find attached Input Component
void UGrabber::MapInput() {
	
	Input = GetOwner()->FindComponentByClass<UInputComponent>();
	if (Input == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("%s missing input component"), *(GetOwner()->GetName()))
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("input component found!"))
			//Bind the input action
			Input->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		Input->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}

FVector UGrabber::PlayerLineTraceStart() {
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerViewPointLocation, PlayerViewPointRotation);
	return PlayerViewPointLocation;	
}

FVector UGrabber::PlayerLineTraceEnd() {
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerViewPointLocation, PlayerViewPointRotation);
	return PlayerViewPointLocation + PlayerViewPointRotation.Vector()*Reach;
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach()  {

	//Ray Cast out to reach distance
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(Hit,PlayerLineTraceStart(),PlayerLineTraceEnd(), FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParams);
	return Hit;
}

void UGrabber::Grab()
{
	if (Handle == nullptr) { return; }
	auto HitInfo = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitInfo.GetComponent();
	auto ActorHit = HitInfo.GetActor();
	if (ActorHit != nullptr) {
		Handle->GrabComponent(ComponentToGrab, NAME_None, ComponentToGrab->GetOwner()->GetActorLocation(), true);
	}
}

void UGrabber::Release() {
	if (Handle == nullptr) { return; }
	Handle->ReleaseComponent();
}

