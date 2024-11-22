#include "LocalMultiplayerSettings.h"
#include "InputMappingContext.h"

bool FLocalMultiplayerProfileData::ContainsKey(const FKey& Key, ELocalMultiplayerInputMappingType MappingType) const
{
	TArray<FEnhancedActionKeyMapping> inputs = GetIMCFromType(MappingType)->GetMappings();
	for (FEnhancedActionKeyMapping Input : inputs)
	{
		if (Input.Key == Key)
		{
			return true;
		}
	}

	return false;
}

UInputMappingContext* FLocalMultiplayerProfileData::GetIMCFromType(ELocalMultiplayerInputMappingType MappingType) const
{
	switch (MappingType)
	{
		case ELocalMultiplayerInputMappingType::InGame:
			return IMCInGame;
		case ELocalMultiplayerInputMappingType::Menu:
			return IMCMenu;
	}
	
	return nullptr;
}

int ULocalMultiplayerSettings::GetNbKeyboardProfiles() const
{
	return NbMaxGamepads;
}

int ULocalMultiplayerSettings::FindKeyboardProfileIndexFromKey(const FKey& Key,
	ELocalMultiplayerInputMappingType MappingType) const
{
	for (int i = 0; i < KeyboardProfileData.Num(); i++)
	{
		if (KeyboardProfileData[i].ContainsKey(Key, MappingType))
		{
			return i;
		}
	}
	
	return -1;
}
