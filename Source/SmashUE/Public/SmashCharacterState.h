#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SmashCharacterState.generated.h"

enum class ESmashCharacterStateID : uint8;
class ASmashCharacter;
class USmashCharacterStateMachine;

UCLASS(Abstract)
class SMASHUE_API USmashCharacterState : public UActorComponent
{
	GENERATED_BODY()

public:	
	USmashCharacterState();

	virtual ESmashCharacterStateID GetStateID();
	virtual void StateInit(USmashCharacterStateMachine* InStateMachine);
	virtual void StateEnter(ESmashCharacterStateID PreviousStateID);
	virtual void StateExit(ESmashCharacterStateID NextStateID);
	virtual void StateTick(float DeltaTime);

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> AnimMontage;

protected:
	UPROPERTY()
	TObjectPtr<ASmashCharacter> Character;

	UPROPERTY()
	TObjectPtr<USmashCharacterStateMachine> StateMachine;
};
