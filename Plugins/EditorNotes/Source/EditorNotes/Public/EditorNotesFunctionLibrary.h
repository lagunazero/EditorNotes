// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EditorNotesFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class EDITORNOTES_API UEditorNotesFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:	
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static AActor* SpawnActorIntoLevel(UObject* WorldContextObject, TSubclassOf<AActor> ActorClass, FString LevelName = "", FVector Location = FVector::ZeroVector, FRotator Rotation = FRotator::ZeroRotator);
};
