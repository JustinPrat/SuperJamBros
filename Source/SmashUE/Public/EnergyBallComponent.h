#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnergyBallComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SMASHUE_API UEnergyBallComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UEnergyBallComponent();
	FVector Direction;
	float Speed;
	
	UFUNCTION(BlueprintCallable)
	void ChargeEnergy();
	
	UFUNCTION(BlueprintCallable)
	void ReleaseEnergy();
	
	UFUNCTION(BlueprintCallable)
	void ThrowBall();

// protected:
// 	virtual void BeginPlay() override;
//
// public:
// 	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
// 	                           FActorComponentTickFunction* ThisTickFunction) override;
};

enum EEnergyBall
{
	None,
	Charging,
	Charged,
	Thrown
};
