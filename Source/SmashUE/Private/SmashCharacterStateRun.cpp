// Fill out your copyright notice in the Description page of Project Settings.


#include "SmashCharacterStateRun.h"
#include "SmashCharacter.h"
#include "SmashCharacterStateID.h"
#include "GameFramework/CharacterMovementComponent.h"

ESmashCharacterStateID USmashCharacterStateRun::GetStateID()
{
	return ESmashCharacterStateID::Run;
}

void USmashCharacterStateRun::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, TEXT("Enter Run"));
}

void USmashCharacterStateRun::StateExit(ESmashCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Exit Run"));
}

void USmashCharacterStateRun::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Green, TEXT("Tick Run"));
	Character->AddMovementInput(Character->GetActorForwardVector());
	Character->GetCharacterMovement()->MaxWalkSpeed = RunSpeedMax;
}
