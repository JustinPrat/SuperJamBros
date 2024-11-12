#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MatchGameMode.generated.h"

class UInputMappingContext;
class USmashCharacterInputData;
class ASmashCharacter;
class AArenaPlayerStart;

UCLASS()
class SMASHUE_API AMatchGameMode : public AGameModeBase
{
	GENERATED_BODY()

	public:
	virtual void BeginPlay() override;

	protected:
	UPROPERTY(EditAnywhere)
	TArray<ASmashCharacter*> CharactersInsideArena;
	
	private:
	USmashCharacterInputData* LoadInputDataFromConfig();
	UInputMappingContext* LoadInputMappingContextFromConfig();
	float LoadInputThresholdFromConfig();
	
	void FindPlayerStartActorsInArena(TArray<AArenaPlayerStart*>& ResultsActors);
	void SpawnCharacters(const TArray<AArenaPlayerStart*>& SpawnPoints);

	TSubclassOf<ASmashCharacter> GetSmashCharacterClassFromInputType(EAutoReceiveInput::Type InputType) const;
};
