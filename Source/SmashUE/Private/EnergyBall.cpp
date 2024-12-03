#include "EnergyBall.h"

#include "SmashCharacter.h"
#include "Physics/ImmediatePhysics/ImmediatePhysicsShared/ImmediatePhysicsCore.h"

AEnergyBall::AEnergyBall()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEnergyBall::BeginPlay()
{
	Super::BeginPlay();
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AEnergyBall::CollideWithOther);
}

void AEnergyBall::CollideWithOther(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "EnergyBall::CollideWithOther : " + OtherActor->GetName());
	ASmashCharacter* Character = Cast<ASmashCharacter>(OtherActor);
	if (Character)
	{
		Character->HitDamage(DamagePercent);
		Character->KnockBack(Direction);
		Destroy();
	}
}

void AEnergyBall::ChargeEnergy(float ratio)
{
	SetActorScale3D(FVector::One() * FMathf::Lerp(MinEnergyScale, MaxEnergyScale, ratio));
	SetActorLocation(GetActorLocation() + Direction * FMathf::Lerp(0, MaxEnergyOffsetPosition, ratio));
}

void AEnergyBall::ReleaseEnergy(float speed, float damage)
{
	SetActorHiddenInGame(false);
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	
	DamagePercent = damage;
	Speed = speed;
	HasBeenThrown = true;
	SetActorEnableCollision(true);
}

void AEnergyBall::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (HasBeenThrown == true)
	{
		SetActorLocation(GetActorLocation() + (Direction * Speed) * DeltaSeconds);
	}
}

