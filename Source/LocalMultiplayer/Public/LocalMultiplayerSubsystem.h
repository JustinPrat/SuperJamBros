#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LocalMultiplayerSubsystem.generated.h"

class ULocalMultiplayerSettings;
enum class ELocalMultiplayerInputMappingType;

UCLASS()
class LOCALMULTIPLAYER_API ULocalMultiplayerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(blueprintCallable)
	void CreateAndInitPlayers(ELocalMultiplayerInputMappingType MappingType);

	int GetAssignedPlayerIndexFromKeyboardProfileIndex(int KeyboardProfileIndex);
	int AssignNewPlayerToKeyboardProfile(int KeyboardProfileIndex);
	void AssignKeyboardMapping(int PlayerIndex, int KeyboardProfileIndex, ELocalMultiplayerInputMappingType MappingType) const;
	int GetAssignedPlayerIndexFromGamepadDeviceID(int DeviceID);
	int AssignNewPlayerToGamepadDeviceID(int DeviceID);
	void AssignGamepadInputMapping(int PlayerIndex, ELocalMultiplayerInputMappingType MappingType) const;
	
	TObjectPtr<const ULocalMultiplayerSettings> LocalMultiplayerSettings;
	
	UPROPERTY()
	uint8 LastAssignedPlayerIndex = -1;

	UPROPERTY()
	TMap<int, int> PlayerIndexFromKeyboardProfileIndex;

	UPROPERTY()
	TMap<int, int> PlayerIndexFromGamepadProfileIndex;
};
