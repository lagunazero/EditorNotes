#pragma once
 
#include "Styling/SlateTypes.h"
#include "Math/Color.h"
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
 
	// The class the notes are using.
	static TSubclassOf<AEditorNoteActor> GetEditorNoteActorClass();
	// The class the notes are using.
	UPROPERTY(config, EditAnywhere, Category = "General")
	TSubclassOf<AEditorNoteActor> EditorNoteActorClass;

	// If not empty, notes are automatically added to this level. Intention is that notes are stored in a separate level layer to avoid disrupting level version control locking. Leave empty to place in persistent level.
	static FString GetNotesLevelName();
	// If not empty, notes are automatically added to this level. Intention is that notes are stored in a separate level layer to avoid disrupting level version control locking. Leave empty to place in persistent level.
	UPROPERTY(config, EditAnywhere, Category = "General")
	FString NotesLevelName;

	// TODO: Not used yet
	static TArray<FLabelData> GetEditorNoteLabels();
	UPROPERTY(config, EditAnywhere, Category = "General")
	TArray<FLabelData> Labels;

	// If all notes should be constantly facing the camera.
	static bool GetAutoRotate();
	// If all notes should be constantly facing the camera.
	UPROPERTY(config, EditAnywhere, Category = "Widget")
	bool bAutoRotate = true;

	// Color of widget background color. Set it here instead of directly in the WBP.
	static FColor GetBackgroundColor();
	// Color of widget background color. Set it here instead of directly in the WBP.
	UPROPERTY(config, EditAnywhere, Category = "Widget")
	FColor BackgroundColor = FColor(0.57f, 0.57f, 0.57f);

	// Color of lines to related actors.
	static FColor GetRelatedActorLineColor();
	// Color of lines to related actors.
	UPROPERTY(config, EditAnywhere, Category = "Widget")
	FColor RelatedActorLineColor = FColor(0.57f, 0.57f, 0.57f);
	// Color of line border to related actors.
	static FColor GetRelatedActorLineColorBorder();
	// Color of line border to related actors.
	UPROPERTY(config, EditAnywhere, Category = "Widget")
	FColor RelatedActorLineColorBorder = FColor(0.29f, 0.29f, 0.29f);

	// Width of lines to related actors.
	static float GetRelatedActorLineWidth();
	// Width of lines to related actors.
	UPROPERTY(config, EditAnywhere, Category = "Widget", meta = (ClampMin = 0))
	float RelatedActorLineWidth = 2.f;
	// Width of line border to related actors.
	static float GetRelatedActorLineWidthBorder();
	// Width of line border to related actors.
	UPROPERTY(config, EditAnywhere, Category = "Widget", meta = (ClampMin = 0))
	float RelatedActorLineWidthBorder = 2.f;

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