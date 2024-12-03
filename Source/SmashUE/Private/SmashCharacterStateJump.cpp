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
	Super::StateEnter(PreviousStateID);
	
	float InitialVelocity = 2 * JumpHeight / (JumpDuration/2);
	float gravity = -2 * JumpHeight / (JumpDuration/2 * JumpDuration/2);
	
	Character->GetCharacterMovement()->GravityScale = gravity / -981;
	Character->GetCharacterMovement()->JumpZVelocity = InitialVelocity;
	
	Character->Jump();
}

void USmashCharacterStateJump::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
	
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, Character->GetCharacterMovement()->IsFalling() ? "falling true" : " falling false");
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, Character->GetVelocity().ToString());
	if(Character->GetVelocity().Z < 0)
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Fall);
	}
}

void USmashCharacterStateJump::StateExit(ESmashCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);
}