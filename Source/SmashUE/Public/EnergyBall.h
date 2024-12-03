#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnergyBall.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SMASHUE_API AEnergyBall : public AActor
{
	GENERATED_BODY()

public:
	AEnergyBall();
	UFUNCTION(BlueprintCallable)
	void ChargeEnergy(float ratio);
	
	UFUNCTION(BlueprintCallable)
	void ReleaseEnergy(float speed, float damage);

	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;

	UFUNCTION()
	void CollideWithOther (UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> SphereComponent;

	FVector Direction;
protected:
	float Speed;
	bool HasBeenThrown = false;

	UPROPERTY(EditAnywhere)
	float MinEnergyScale = 0.1f;
	UPROPERTY(EditAnywhere)
	float MaxEnergyScale = 1.0f;
	
	UPROPERTY(EditAnywhere)
	float MaxEnergyOffsetPosition = 1.0f;

	float DamagePercent;
};