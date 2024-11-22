#include "LocalMultiplayerGameViewportClient.h"
#include "LocalMultiplayerSettings.h"
#include "LocalMultiplayerSubsystem.h"
#include "GameFramework/PlayerInput.h"
#include "Kismet/GameplayStatics.h"

void ULocalMultiplayerGameViewportClient::PostInitProperties()
{
	Super::PostInitProperties();
	MaxSplitscreenPlayers = 8;
	LocalMultiplayerSettings = GetDefault<ULocalMultiplayerSettings>();
}

bool ULocalMultiplayerGameViewportClient::InputKey(const FInputKeyEventArgs& EventArgs)
{
	if (!LocalMultiplayerSettings) return false;
	ULocalMultiplayerSubsystem* LocalMultiplayerSubsystem = GetGameInstance()->GetSubsystem<ULocalMultiplayerSubsystem>();

	if (EventArgs.IsGamepad())
	{
		int GamePadID = EventArgs.InputDevice.GetId();
		int PlayerID = LocalMultiplayerSubsystem->GetAssignedPlayerIndexFromGamepadDeviceID(GamePadID);
		if (PlayerID == -1)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, "Keyboard connected");
			int NewPlayerID = LocalMultiplayerSubsystem->AssignNewPlayerToGamepadDeviceID(GamePadID);
			LocalMultiplayerSubsystem->AssignGamepadInputMapping(NewPlayerID, ELocalMultiplayerInputMappingType::InGame);
		}
		else
		{
			APlayerController* PlayerController = UGameplayStatics::GetPlayerControllerFromID(GetWorld(), PlayerID);
			FInputKeyParams InputKeyParams (EventArgs.Key, EventArgs.Event, EventArgs.AmountDepressed, EventArgs.IsGamepad(), EventArgs.InputDevice);
			return PlayerController->InputKey(InputKeyParams);
		}
	}
	else
	{
		int KeyBoardID = LocalMultiplayerSettings->FindKeyboardProfileIndexFromKey(EventArgs.Key, ELocalMultiplayerInputMappingType::InGame);
		if (KeyBoardID == -1) return Super::InputKey(EventArgs);
		
		int PlayerID = LocalMultiplayerSubsystem->GetAssignedPlayerIndexFromKeyboardProfileIndex(KeyBoardID);
		if (PlayerID == -1)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, "Keyboard connected");
			int NewPlayerID = LocalMultiplayerSubsystem->AssignNewPlayerToKeyboardProfile(KeyBoardID);
			LocalMultiplayerSubsystem->AssignKeyboardMapping(NewPlayerID, KeyBoardID, ELocalMultiplayerInputMappingType::InGame);
		}
		else
		{
			APlayerController* PlayerController = UGameplayStatics::GetPlayerControllerFromID(GetWorld(), PlayerID);
			FInputKeyParams InputKeyParams (EventArgs.Key, EventArgs.Event, EventArgs.AmountDepressed, EventArgs.IsGamepad(), EventArgs.InputDevice);
			return PlayerController->InputKey(InputKeyParams);
		}
	}
	
	return Super::InputKey(EventArgs);
}

bool ULocalMultiplayerGameViewportClient::InputAxis(FViewport* InViewport, FInputDeviceId InputDevice, FKey Key,
	float Delta, float DeltaTime, int32 NumSamples, bool bGamepad)
{
	if (!LocalMultiplayerSettings) return false;
	ULocalMultiplayerSubsystem* LocalMultiplayerSubsystem = GetGameInstance()->GetSubsystem<ULocalMultiplayerSubsystem>();
	
	int PlayerID = LocalMultiplayerSubsystem->GetAssignedPlayerIndexFromGamepadDeviceID(InputDevice.GetId());
	if (bGamepad && PlayerID == -1)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, "Gamepad connected");
		int NewPlayerID = LocalMultiplayerSubsystem->AssignNewPlayerToGamepadDeviceID(InputDevice.GetId());
		LocalMultiplayerSubsystem->AssignGamepadInputMapping(NewPlayerID, ELocalMultiplayerInputMappingType::InGame);
	}
	else
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerControllerFromID(GetWorld(), PlayerID);
		FInputKeyParams InputKeyParams (Key, IE_Axis, Delta, true, InputDevice);
		return PlayerController->InputKey(InputKeyParams);
	}
	
	return Super::InputAxis(InViewport, InputDevice, Key, Delta, DeltaTime, NumSamples, bGamepad);
}
