#pragma once

#include "GameFramework/Actor.h"
#include "EditorNotesSettings.h"
#include "EditorNoteActor.generated.h"

class UWidgetComponent;

UENUM(BlueprintType)
enum class ENotePrioType : uint8
{
	NPT_LOW      UMETA(DisplayName = "Low"),
	NPT_MID      UMETA(DisplayName = "Mid"),
	NPT_HIGH     UMETA(DisplayName = "High"),
	NPT_COMMENT  UMETA(DisplayName = "Comment"),
};

/**
 * The actor that gets placed in the scenes. Should not get added manually, only using the plugin's means.
 */
UCLASS(Abstract, Blueprintable, Config = EditorNotes, AutoExpandCategories = (EditorNotes))
class EDITORNOTES_API AEditorNoteActor : public AActor
{
	GENERATED_BODY()

public:
	AEditorNoteActor();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	// Allow tick in editor, for auto-rotation and widget content updating.
	virtual bool ShouldTickIfViewportsOnly() const override { return true; }

	void InitNote();
	void ResetDate();
	void ResetUser();
	void ResetPosition();

	void ShowRelatedActors();
	void AutoRotate();

	UFUNCTION(BlueprintImplementableEvent, meta = (CallInEditor = "true"))
	void UpdateText();
	UFUNCTION(BlueprintImplementableEvent, meta = (CallInEditor = "true"))
	void OnSelectionChange(bool bSelected);

	UFUNCTION(BlueprintCallable)
	FString GetPrioAsString() const;

	// Name of the note. If blank, the widget shows the actor's name instead.
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	FString Title = "";
	// Any opinions on the level? Put those thoughts here!
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (MultiLine = true))
	FString Text = "";
	// The note's priority.
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	ENotePrioType Prio;
	// If this note has been resolved.
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	bool bResolved = false;

	// Connect any actors related to this note. Will show lines to them when note is selected.
	UPROPERTY(EditInstanceOnly)
	TArray<TSoftObjectPtr<AActor> > RelatedActors;

	// The name of the creator of this note. Automatically set on creation.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FString Creator = "";
	// The date this note was created. Automatically set on creation.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FDateTime Date = FDateTime();

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	bool bArt = false;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	bool bCode = false;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	bool bDesign = false;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	bool bLD = false;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	bool bWriting = false;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	bool bAudio = false;
	//UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	//TArray<FLabelData> Labels;

protected:

	// Root component so widget can be offset.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* Root;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UWidgetComponent* Widget;

	bool bWasSelected = false;
	bool bHasTicked = false;
};
