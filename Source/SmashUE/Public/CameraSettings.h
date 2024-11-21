#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "CameraSettings.generated.h"

UCLASS(Config=Game, DefaultConfig, meta=(DisplayName = "Camera Settings"))
class SMASHUE_API UCameraSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category="Camera")
	FName CameraMainTag;

	UPROPERTY(Config, EditAnywhere, Category="Camera")
	FName CameraDistanceMinTag;

	UPROPERTY(Config, EditAnywhere, Category="Camera")
	FName CameraDistanceMaxTag;

	UPROPERTY(Config, EditAnywhere, Category="Camera")
	FName CameraBoundsTag;

	UPROPERTY(Config, EditAnywhere, Category="Camera")
	float CameraDistanceBetweenTargetsMin;

	UPROPERTY(Config, EditAnywhere, Category="Camera")
	float CameraDistanceBetweenTargetsMax;

	UPROPERTY(Config, EditAnywhere, Category="Camera")
	float PositionDampingFactor;

	UPROPERTY(Config, EditAnywhere, Category="Camera")
	float SizeDampingFactor;
};
