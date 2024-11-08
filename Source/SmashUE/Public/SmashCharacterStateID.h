#pragma once
#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ESmashCharacterStateID : uint8
{
	None = 0,
	Idle,
	Walk,
	Run
};