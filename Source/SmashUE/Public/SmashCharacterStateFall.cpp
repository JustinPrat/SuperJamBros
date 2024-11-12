#include "SmashCharacterStateFall.h"

#include "SmashCharacter.h"
#include "SmashCharacterStateID.h"
#include "SmashCharacterStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"

ESmashCharacterStateID USmashCharacterStateFall::GetStateID()
{
	return ESmashCharacterStateID::Fall;
}

void USmashCharacterStateFall::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);
}

void USmashCharacterStateFall::StateExit(ESmashCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);
}

void USmashCharacterStateFall::StateTick(float DeltaTime)
{
	//GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Green, TEXT("Tick Fall"));
	if (!Character->GetCharacterMovement()->IsFalling())
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Idle);
	}
	Super::StateTick(DeltaTime);
}
