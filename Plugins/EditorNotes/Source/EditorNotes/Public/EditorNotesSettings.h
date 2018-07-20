#pragma once
 
#include "Styling/SlateTypes.h"
#include "EditorNotesSettings.generated.h"

class AEditorNoteActor;

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
	static FString GetNotesLevelName();
	static const FScrollBoxStyle& GetWidgetListStyle();

	/** Sample bool property */
	UPROPERTY(EditAnywhere, config, Category = Custom)
	bool bSampleBool;
 
	/** Sample float property that requires a restart */
	UPROPERTY(EditAnywhere, config, Category = Custom, meta = (ConfigRestartRequired = true))
	float SampleFloatRequireRestart;
 
	/** Sample string list */
	UPROPERTY(config, EditAnywhere, Category = Custom)
	TArray<FString> SampleStringList;
 
	/** Or add min, max or clamp values to the settings */
	UPROPERTY(config, EditAnywhere, Category = Custom, meta = (UIMin = 1, ClampMin = 1))
	int32 ClampedIntSetting;
 
	/** We can even use asset references */
	UPROPERTY(config, EditAnywhere, Category = Materials, meta = (AllowedClasses = "MaterialInterface"))
	FStringAssetReference StringMaterialAssetReference;
 
	UPROPERTY(config, EditAnywhere, Category = "General")
	TSubclassOf<AEditorNoteActor> EditorNoteActorClass;

	UPROPERTY(config, EditAnywhere, Category = "General")
	FString NotesLevelName;

	UPROPERTY(config, EditAnywhere, Category = "General")
	FScrollBoxStyle WidgetListStyle;
};