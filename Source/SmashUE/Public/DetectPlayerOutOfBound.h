// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/BoxComponent.h"
#include "DetectPlayerOutOfBound.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SMASHUE_API UDetectPlayerOutOfBound : public UActorComponent
{
	GENERATED_BODY()

public:
	UDetectPlayerOutOfBound();

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UBoxComponent> BoxComponent;

	UFUNCTION()
	void CollideWithOther(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
