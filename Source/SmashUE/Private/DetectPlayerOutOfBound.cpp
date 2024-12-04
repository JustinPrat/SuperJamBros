// Fill out your copyright notice in the Description page of Project Settings.


#include "DetectPlayerOutOfBound.h"

#include <filesystem>

#include "CameraWorldSubsystem.h"
#include "SmashCharacter.h"


// Sets default values for this component's properties
UDetectPlayerOutOfBound::UDetectPlayerOutOfBound()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UDetectPlayerOutOfBound::CollideWithOther(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ASmashCharacter* Character = Cast<ASmashCharacter>(OtherActor);
	if (Character)
	{
		Character->Destroy();
		GetWorld()->GetSubsystem<UCameraWorldSubsystem>()->RemoveFollowTarget(Character);
	}
}

void UDetectPlayerOutOfBound::BeginPlay()
{
	Super::BeginPlay();
	
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &UDetectPlayerOutOfBound::CollideWithOther);
}

void UDetectPlayerOutOfBound::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

