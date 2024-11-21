#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Subsystems/WorldSubsystem.h"
#include "CameraWorldSubsystem.generated.h"

class UCameraSettings;

UCLASS()
class SMASHUE_API UCameraWorldSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void PostInitialize() override;
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override { return TStatId(); }
	void AddFollowTarget(UObject* FollowTarget);
	void RemoveFollowTarget(UObject* FollowTarget);

protected:
	UPROPERTY()
	TObjectPtr<UCameraComponent> CameraMain;

	UPROPERTY()
	TArray<UObject*> FollowTargets;

	UPROPERTY()
	FVector2D CameraBoundsMin;
	
	UPROPERTY()
	FVector2D CameraBoundsMax;
	
	UPROPERTY()
	float CameraBoundsYProjectionCenter;

	AActor* FindCameraBoundsActor();

	void InitCameraBounds(AActor* CameraBoundsActor);
	void ClampPositionIntoCameraBounds(FVector& Position);

	void GetViewportBounds(FVector2D& OutViewportBoundsMin, FVector2D& OutViewportBoundsMax);
	FVector CalculateWorldPositionFromViewportPosition(const FVector2D& ViewportPosition);
	

	void TickUpdateCameraPosition(float DeltaTime);
	void TickUpdateCameraZoom(float DeltaTime);
	FVector CalculateAveragePositionBetweenTargets();
	float CalculateGreatestDistanceBetweenTargets();
	UCameraComponent* FindCameraByTag(const FName& Tag) const;
	TObjectPtr<const UCameraSettings> CameraSettings;

protected:
	UPROPERTY()
	float CameraZoomYMin = 0.0f;

	UPROPERTY()
	float CameraZoomYMax = 0.0f;

	UPROPERTY()
	float CameraZoomDistanceBetweenTargetsMin = 300.0f;
	
	UPROPERTY()
	float CameraZoomDistanceBetweenTargetsMax = 1500.0f;

	UFUNCTION()
	void InitCameraZoomParameters();
};
