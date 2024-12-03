#pragma once

#include "CoreMinimal.h"
class SMASHUE_API EEnergyBallState
{
public:
	EEnergyBallState();
	~EEnergyBallState();

	enum EEnergyBall
	{
		None,
		Charging,
		Charged,
		Thrown
	};
};
