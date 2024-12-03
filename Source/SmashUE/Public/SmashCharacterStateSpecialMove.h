#pragma once

#include "CoreMinimal.h"
#include "EnergyBall.h"
#include "EnergyBallComponent.h"
#include "SmashCharacterState.h"
#include "SmashCharacterStateSpecialMove.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSpecialMove_StartSpecialMove, ESmashCharacterStateID, PreviousStateID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSpecialMove_ExitSpecialMove, ESmashCharacterStateID, NextStateID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSpecialMove_TickSpecialMove, float, DeltaTime);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SMASHUE_API USmashCharacterStateSpecialMove : public USmashCharacterState
{
	GENERATED_BODY()

public:
	virtual ESmashCharacterStateID GetStateID() override;

	virtual void StateInit(USmashCharacterStateMachine* InStateMachine) override;
	virtual void StateEnter(ESmashCharacterStateID PreviousStateID) override;
	virtual void StateExit(ESmashCharacterStateID NextStateID) override;
	virtual void StateTick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AEnergyBall> EnergyBall;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<AEnergyBall> EnergyBallInstance;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> Gun;
	
	UPROPERTY(BlueprintAssignable)
	FSpecialMove_StartSpecialMove StartSpecialMoveDelegate;
	UPROPERTY(BlueprintAssignable)
	FSpecialMove_ExitSpecialMove ExitSpecialMoveDelegate;
	UPROPERTY(BlueprintAssignable)
	FSpecialMove_TickSpecialMove TickSpecialMoveDelegate;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> ActionAnim;

	UFUNCTION()
	void OnInputSpecialMove();

private:
	float TimeSinceStartCharge;
	
	UPROPERTY(EditAnywhere)
	float TimeToCharge;

	UPROPERTY(EditAnywhere)
	float BallSpeed;

	UPROPERTY(EditAnywhere)
	float MinDamage;
	UPROPERTY(EditAnywhere)
	float MaxDamage;

	EEnergyBall EnergyBallState;

	void GenerateNewBall();
	void Shoot();
};
