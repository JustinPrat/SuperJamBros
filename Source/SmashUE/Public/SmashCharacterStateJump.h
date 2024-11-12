#pragma once

#include "CoreMinimal.h"
#include "SmashCharacterState.h"
#include "SmashCharacterStateJump.generated.h"

UCLASS(ClassGroup=(SmashCharacterState), meta=(BlueprintSpawnableComponent))
class SMASHUE_API USmashCharacterStateJump : public USmashCharacterState
{
	GENERATED_BODY()
	
public:
	virtual ESmashCharacterStateID GetStateID() override;
	
	virtual void StateEnter(ESmashCharacterStateID PreviousStateID) override;
	virtual void StateExit(ESmashCharacterStateID NextStateID) override;
	virtual void StateTick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	float JumpHeight;
	
	UPROPERTY(EditAnywhere)
	float JumpDuration;
};
