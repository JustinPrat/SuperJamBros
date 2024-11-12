#include "SmashCharacterStateJump.h"
#include "SmashCharacter.h"
#include "SmashCharacterStateID.h"
#include "SmashCharacterStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"

ESmashCharacterStateID USmashCharacterStateJump::GetStateID()
{
	return ESmashCharacterStateID::Jump;
}

void USmashCharacterStateJump::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	//Character->AddMovementInput(FVector::UpVector, JumpHeight/JumpDuration);
	Character->Jump();
}

void USmashCharacterStateJump::StateTick(float DeltaTime)
{
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, Character->GetCharacterMovement()->IsFalling() ? "falling true" : " falling false");
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, Character->GetVelocity().ToString());
	if(Character->GetVelocity().Z < 0)
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Fall);
	}
}

void USmashCharacterStateJump::StateExit(ESmashCharacterStateID NextStateID)
{
	
}