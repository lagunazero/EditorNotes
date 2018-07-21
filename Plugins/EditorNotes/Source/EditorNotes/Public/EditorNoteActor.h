#pragma once

#include "GameFramework/Actor.h"
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

	virtual void Tick(float DeltaSeconds) override;
	// Allow tick in editor, for auto-rotation.
	virtual bool ShouldTickIfViewportsOnly() const override { return true; }

	void InitNote();
	void ResetDate();
	void ResetUser();
	void ResetPosition();

	UFUNCTION(BlueprintImplementableEvent, meta = (CallInEditor = "true"))
	void UpdateText();

	FString PrioAsString() const;

	// Any opinions on the level? Put those thoughts here!
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Text = "";
	// The note's priority.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ENotePrioType Prio;
	// If this note has been resolved.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bResolved = false;

	// The name of the creator of this note. Automatically set on creation.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FString Creator = "";
	// The date this note was created. Automatically set on creation.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FDateTime Date = FDateTime();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bLevelDesign = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bArt = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAudio = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCode = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bWriting = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bDesign = false;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UWidgetComponent* Widget;
};
