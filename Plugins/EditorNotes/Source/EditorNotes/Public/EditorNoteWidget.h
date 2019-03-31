// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UserWidget.h"
#include "EditorNoteWidget.generated.h"

class AEditorNoteActor;

/**
 * 
 */
UCLASS()
class /*EDITORNOTES_API*/ UEditorNoteWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	//UFUNCTION(BlueprintPure)
	//FColor GetBackgroundColor() const;

	//UPROPERTY(BlueprintCallable)
	void SetSelected(bool bSelected);

	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<AEditorNoteActor> Note;
	UPROPERTY(BlueprintReadOnly)
	bool bSelected = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float ResolvedTransparency = 0.5f;
};
