// Fill out your copyright notice in the Description page of Project Settings.


#include "MatchGameMode.h"
#include "SmashCharacter.h"
#include "Arena/ArenaPlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "ArenaSettings.h"
#include "SmashCharacterSettings.h"
#include "InputMappingContext.h"
#include "LocalMultiplayerSettings.h"
#include "LocalMultiplayerSubsystem.h"

void AMatchGameMode::BeginPlay()
{
	Super::BeginPlay();
	CreateAndInitPlayers();

	TArray<AArenaPlayerStart*> PlayersStartsPoints;
	FindPlayerStartActorsInArena(PlayersStartsPoints);
	SpawnCharacters(PlayersStartsPoints);

	for (AArenaPlayerStart* PlayerStart : PlayersStartsPoints)
	{
		EAutoReceiveInput::Type InputType = PlayerStart->AutoReceiveInput.GetValue();
		TSubclassOf<ASmashCharacter> SmashCharacterClass = GetSmashCharacterClassFromInputType(InputType);
		if (SmashCharacterClass == nullptr) continue;
		
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, SmashCharacterClass->GetFName().ToString());
	}
}

USmashCharacterInputData* AMatchGameMode::LoadInputDataFromConfig()
{
	const USmashCharacterSettings* CharacterSettings = GetDefault<USmashCharacterSettings>();
	if (CharacterSettings == nullptr) return nullptr;
	return CharacterSettings->InputData.LoadSynchronous();
}

float AMatchGameMode::LoadInputThresholdFromConfig()
{
	const USmashCharacterSettings* CharacterSettings = GetDefault<USmashCharacterSettings>();
	if (CharacterSettings == nullptr) return 0.1f;
	return CharacterSettings->InputXThreshold;
}

void AMatchGameMode::CreateAndInitPlayers() const
{
	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	if (GameInstance == nullptr) return;

	ULocalMultiplayerSubsystem* LocalMultiplayerSubsystem = GameInstance->GetSubsystem<ULocalMultiplayerSubsystem>();
	if (LocalMultiplayerSubsystem == nullptr) return;

	LocalMultiplayerSubsystem->CreateAndInitPlayers(ELocalMultiplayerInputMappingType::InGame);
}


UInputMappingContext* AMatchGameMode::LoadInputMappingContextFromConfig()
{
	const USmashCharacterSettings* CharacterSettings = GetDefault<USmashCharacterSettings>();
	if (CharacterSettings == nullptr) return nullptr;
	return CharacterSettings->InputMappingContext.LoadSynchronous();
}

void AMatchGameMode::FindPlayerStartActorsInArena(TArray<AArenaPlayerStart*>& ResultsActors)
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AArenaPlayerStart::StaticClass(), FoundActors);

	for (int i = 0; i < FoundActors.Num(); i++)
	{
		AArenaPlayerStart* ArenaPlayerStartActor = Cast<AArenaPlayerStart>(FoundActors[i]);
		if (ArenaPlayerStartActor == nullptr) continue;

		ResultsActors.Add(ArenaPlayerStartActor);
	}
}

void AMatchGameMode::SpawnCharacters(const TArray<AArenaPlayerStart*>& SpawnPoints)
{
	USmashCharacterInputData* InputData = LoadInputDataFromConfig();
	UInputMappingContext* InputMappingContext = LoadInputMappingContextFromConfig();
	float InputMoveXThreshold = LoadInputThresholdFromConfig();
	
	for (AArenaPlayerStart* SpawnPoint : SpawnPoints)
	{
		EAutoReceiveInput::Type InputType = SpawnPoint->AutoReceiveInput.GetValue();
		TSubclassOf<ASmashCharacter> SmashCharacterClass = GetSmashCharacterClassFromInputType(InputType);
		if (SmashCharacterClass == nullptr) continue;

		ASmashCharacter* NewCharacter = GetWorld()->SpawnActorDeferred<ASmashCharacter>(
			SmashCharacterClass,
			SpawnPoint->GetTransform()
		);

		if (NewCharacter == nullptr) continue;

		NewCharacter->InputData = InputData;
		NewCharacter->InputMappingContext = InputMappingContext;
		NewCharacter->AutoPossessPlayer = SpawnPoint->AutoReceiveInput;
		NewCharacter->SetOrientX(SpawnPoint->GetStartOrientX());
		NewCharacter->FinishSpawning(SpawnPoint->GetTransform());
		NewCharacter->InputMoveXThreshold = InputMoveXThreshold;

		CharactersInsideArena.Add(NewCharacter);
	}
}

TSubclassOf<ASmashCharacter> AMatchGameMode::GetSmashCharacterClassFromInputType(EAutoReceiveInput::Type InputType) const
{
	const UArenaSettings* ArenaSettings = GetDefault<UArenaSettings>();

	switch (InputType) {
	case EAutoReceiveInput::Player0:
		return ArenaSettings->SmashCharacterClassP0;
	case EAutoReceiveInput::Player1:
		return ArenaSettings->SmashCharacterClassP1;
	case EAutoReceiveInput::Player2:
		return ArenaSettings->SmashCharacterClassP2;
	case EAutoReceiveInput::Player3:
		return ArenaSettings->SmashCharacterClassP3;
		
	default:
		return nullptr;
	}
}

