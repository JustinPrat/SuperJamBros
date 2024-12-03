#include "SmashCharacterStateSpecialMove.h"

#include "SmashCharacter.h"
#include "SmashCharacterStateID.h"
#include "SmashCharacterStateMachine.h"

ESmashCharacterStateID USmashCharacterStateSpecialMove::GetStateID()
{
	return ESmashCharacterStateID::SpecialMove;
}

void USmashCharacterStateSpecialMove::StateInit(USmashCharacterStateMachine* InStateMachine)
{
	Super::StateInit(InStateMachine);
	GenerateNewBall();
}

void USmashCharacterStateSpecialMove::Shoot()
{
	Character->PlayAnimMontage(ActionAnim);
	FVector dir = FVector(Character->GetOrientX(), 0, 0);
	dir.Normalize();
	EnergyBallInstance->ReleaseEnergy(BallSpeed, FMathf::Lerp(MinDamage, MaxDamage, TimeSinceStartCharge/TimeToCharge));
	GenerateNewBall();
	
	StateMachine->ChangeState(ESmashCharacterStateID::Idle);
}

void USmashCharacterStateSpecialMove::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);
	Character->InputSpecialEvent.AddDynamic(this, &USmashCharacterStateSpecialMove::OnInputSpecialMove);
	StartSpecialMoveDelegate.Broadcast(PreviousStateID);
	
	if (EnergyBallState == Charged)
	{
		EnergyBallState = Charging;
		Shoot();
	}
	else
	{
		EnergyBallState = Charging;
		TimeSinceStartCharge = 0;
	}
}

void USmashCharacterStateSpecialMove::StateExit(ESmashCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);
	Character->InputSpecialEvent.RemoveDynamic(this, &USmashCharacterStateSpecialMove::OnInputSpecialMove);
	ExitSpecialMoveDelegate.Broadcast(NextStateID);
}

void USmashCharacterStateSpecialMove::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
	if (EnergyBallState == Charged) return;
	
	EnergyBallInstance->ChargeEnergy(TimeSinceStartCharge/ TimeToCharge);
	TimeSinceStartCharge += DeltaTime;
	
	if (TimeSinceStartCharge >= TimeToCharge)
	{
		EnergyBallState = Charged;
	}
	
	if (EnergyBallState == Charged)
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Idle);
	}
	
	TickSpecialMoveDelegate.Broadcast(DeltaTime);
}

void USmashCharacterStateSpecialMove::OnInputSpecialMove()
{
	if (EnergyBallState == Charging)
	{
		Shoot();
	}
}

void USmashCharacterStateSpecialMove::GenerateNewBall()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector dir = FVector(Character->GetOrientX(), 0, 0);
	dir.Normalize();
	
	EnergyBallInstance = GetWorld()->SpawnActor<AEnergyBall>(EnergyBall, SpawnParams);
	EnergyBallInstance->AttachToComponent(Gun, FAttachmentTransformRules::SnapToTargetNotIncludingScale, "GunSocket");
	EnergyBallInstance->SetActorScale3D(FVector::Zero());
	EnergyBallInstance->SetActorEnableCollision(false);
	EnergyBallInstance->Direction = dir;
}