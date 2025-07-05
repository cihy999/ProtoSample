// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProtoSamplePickUpComponent.h"

UProtoSamplePickUpComponent::UProtoSamplePickUpComponent()
{
	// Setup the Sphere Collision
	SphereRadius = 32.f;
}

void UProtoSamplePickUpComponent::BeginPlay()
{
	Super::BeginPlay();

	// Register our Overlap Event
	OnComponentBeginOverlap.AddDynamic(this, &UProtoSamplePickUpComponent::OnSphereBeginOverlap);
}

void UProtoSamplePickUpComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Checking if it is a First Person Character overlapping
	AProtoSampleCharacter* Character = Cast<AProtoSampleCharacter>(OtherActor);
	if(Character != nullptr)
	{
		// Notify that the actor is being picked up
		OnPickUp.Broadcast(Character);

		// Unregister from the Overlap Event so it is no longer triggered
		OnComponentBeginOverlap.RemoveAll(this);
	}
}
