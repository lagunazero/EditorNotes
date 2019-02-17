#pragma once
 
#include "Styling/SlateTypes.h"
#include "EditorNotesSettings.generated.h"

class AEditorNoteActor;

USTRUCT(Blueprintable)
struct FLabelData
{
	GENERATED_BODY()

	FLabelData() {}
	FLabelData(FString Name, FLinearColor Color)
	{
		this->Name = Name;
		this->Color = Color;
	}

	UPROPERTY(config, EditAnywhere)
	FString Name;
	UPROPERTY(config, EditAnywhere)
	FLinearColor Color;
};

/**
 * Setting object used to hold both config settings and editable ones in one place
 * To ensure the settings are saved to the specified config file make sure to add
 * props using the globalconfig or config meta.
 */
UCLASS(config = Game, defaultconfig)
class UEditorNotesSettings : public UObject
{
	GENERATED_BODY()
 
public:
	UEditorNotesSettings(const FObjectInitializer& ObjectInitializer);
 
	static TSubclassOf<AEditorNoteActor> GetEditorNoteActorClass();
	UPROPERTY(config, EditAnywhere, Category = "General")
	TSubclassOf<AEditorNoteActor> EditorNoteActorClass;

	static FString GetNotesLevelName();
	UPROPERTY(config, EditAnywhere, Category = "General")
	FString NotesLevelName;

	static bool GetAutoRotate();
	UPROPERTY(config, EditAnywhere, Category = "General")
	bool bAutoRotate = true;

	// TODO: Not used yet
	static TArray<FLabelData> GetEditorNoteLabels();
	UPROPERTY(config, EditAnywhere, Category = "General")
	TArray<FLabelData> Labels;

	/*
	// Sample bool property
	UPROPERTY(EditAnywhere, config, Category = Custom)
	bool bSampleBool;
 
	// Sample float property that requires a restart
	UPROPERTY(EditAnywhere, config, Category = Custom, meta = (ConfigRestartRequired = true))
	float SampleFloatRequireRestart;
 
	// Sample string list
	UPROPERTY(config, EditAnywhere, Category = Custom)
	TArray<FString> SampleStringList;

	// Or add min, max or clamp values to the settings
	UPROPERTY(config, EditAnywhere, Category = Custom, meta = (UIMin = 1, ClampMin = 1))
	int32 ClampedIntSetting;
 
	// We can even use asset references
	UPROPERTY(config, EditAnywhere, Category = Materials, meta = (AllowedClasses = "MaterialInterface"))
	FStringAssetReference StringMaterialAssetReference;
	*/
};