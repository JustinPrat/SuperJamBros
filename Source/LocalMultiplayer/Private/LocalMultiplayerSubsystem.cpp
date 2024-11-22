// Fill out your copyright notice in the Description page of Project Settings.


#include "LocalMultiplayerSubsystem.h"
#include "EnhancedInputSubsystems.h"
#include "LocalMultiplayerSettings.h"
#include "Kismet/GameplayStatics.h"


void ULocalMultiplayerSubsystem::CreateAndInitPlayers(ELocalMultiplayerInputMappingType MappingType)
{
	LocalMultiplayerSettings = GetDefault<ULocalMultiplayerSettings>();

	for (int i = 0; i < LocalMultiplayerSettings->NbMaxGamepads * 2; i++)
	{
		UGameplayStatics::CreatePlayer(GetWorld(), i);
	}
}

int ULocalMultiplayerSubsystem::GetAssignedPlayerIndexFromKeyboardProfileIndex(int KeyboardProfileIndex)
{
	int* result = PlayerIndexFromKeyboardProfileIndex.Find(KeyboardProfileIndex);
	if (result != nullptr)
	{
		return *result;
	}
	
	return -1;
}

int ULocalMultiplayerSubsystem::AssignNewPlayerToKeyboardProfile(int KeyboardProfileIndex)
{
	LastAssignedPlayerIndex++;
	
	if (GetAssignedPlayerIndexFromKeyboardProfileIndex(KeyboardProfileIndex) != -1)
	{
		PlayerIndexFromKeyboardProfileIndex[KeyboardProfileIndex] = LastAssignedPlayerIndex;
	}
	else
	{
		PlayerIndexFromKeyboardProfileIndex.Add(KeyboardProfileIndex, LastAssignedPlayerIndex);
	}
	
	return LastAssignedPlayerIndex;
}

void ULocalMultiplayerSubsystem::AssignKeyboardMapping(int PlayerIndex, int KeyboardProfileIndex,
	ELocalMultiplayerInputMappingType MappingType) const
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerControllerFromID(GetWorld(), PlayerIndex);

	if(!PlayerController) return;

	ULocalPlayer* Player = PlayerController->GetLocalPlayer();
	if(!Player) return;

	UEnhancedInputLocalPlayerSubsystem* InputSystem = Player->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	UInputMappingContext* MappingContext = LocalMultiplayerSettings->KeyboardProfileData[KeyboardProfileIndex].GetIMCFromType(MappingType);
	InputSystem->AddMappingContext(MappingContext, 0);
}

int ULocalMultiplayerSubsystem::GetAssignedPlayerIndexFromGamepadDeviceID(int DeviceID)
{
	int* result = PlayerIndexFromGamepadProfileIndex.Find(DeviceID);
	if (result != nullptr)
	{
		return *result;
	}
	
	return -1;
}

int ULocalMultiplayerSubsystem::AssignNewPlayerToGamepadDeviceID(int DeviceID)
{
	LastAssignedPlayerIndex++;
	
	if (GetAssignedPlayerIndexFromGamepadDeviceID(DeviceID) != -1)
	{
		PlayerIndexFromGamepadProfileIndex[DeviceID] = LastAssignedPlayerIndex;
	}
	else
	{
		PlayerIndexFromGamepadProfileIndex.Add(DeviceID, LastAssignedPlayerIndex);
	}
	
	return LastAssignedPlayerIndex;
}

void ULocalMultiplayerSubsystem::AssignGamepadInputMapping(int PlayerIndex, ELocalMultiplayerInputMappingType MappingType) const
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerControllerFromID(GetWorld(), PlayerIndex);

	if(!PlayerController) return;

	ULocalPlayer* Player = PlayerController->GetLocalPlayer();
	if(!Player) return;

	UEnhancedInputLocalPlayerSubsystem* InputSystem = Player->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

	InputSystem->AddMappingContext(LocalMultiplayerSettings->GamepadProfileData.GetIMCFromType(MappingType), 0);
}

