// Fill out your copyright notice in the Description page of Project Settings.

#include "EditorNotesFunctionLibrary.h"

AActor* UEditorNotesFunctionLibrary::SpawnActorIntoLevel(UObject* WorldContextObject, TSubclassOf<AActor> ActorClass, FString LevelName, FVector Location, FRotator Rotation)
{
	if (!ActorClass) return nullptr;
	if (!GEngine) return nullptr;

	//using a context object to get the world
	UWorld* const World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
	if (!World) return nullptr;

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParameters.bDeferConstruction = false;

	//Get Level from Name
	ULevel* FoundLevel = nullptr;

	if (!LevelName.IsEmpty())
	{
		for (const ULevelStreaming* EachLevel : World->StreamingLevels)
		{
			if (!EachLevel) continue;
			ULevel* LevelPtr = EachLevel->GetLoadedLevel();
			if (!LevelPtr) continue;

			const FString PackageName = EachLevel->GetWorldAssetPackageName();
			TArray<FString> Tokens;
			const TCHAR delim = '/'; // TODO: Make platform independent
			PackageName.ParseIntoArray(Tokens, &delim);
			if (Tokens.Num() > 0 && Tokens.Last().Equals(LevelName))
			{
				FoundLevel = LevelPtr;
				break;
			}
		}
	}
	else
	{
		FoundLevel = World->PersistentLevel;
	}

	if (!FoundLevel)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to spawn actor in level \"%s\". Level not found."), *LevelName);
		return nullptr;
	}

	SpawnParameters.OverrideLevel = FoundLevel;
	return World->SpawnActor(ActorClass, &Location, &Rotation, SpawnParameters);
}
