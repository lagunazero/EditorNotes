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

UCLASS(Abstract, Blueprintable, Config = EditorNotes, AutoExpandCategories = (EditorNotes))
class EDITORNOTES_API AEditorNoteActor : public AActor
{
	GENERATED_BODY()

public:
	AEditorNoteActor();

	virtual void Tick(float DeltaSeconds) override;
	virtual bool ShouldTickIfViewportsOnly() const override { return true; }

	UFUNCTION(BlueprintCallable, meta = (CallInEditor = "true"))
	void InitNote();
	void ResetDate();
	void ResetUser();
	void ResetPosition();

	UFUNCTION(BlueprintImplementableEvent, meta = (CallInEditor = "true"))
	void UpdateText();

	FString PrioAsString() const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UWidgetComponent* Widget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Text = "";
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Creator = "";
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FDateTime Date = FDateTime();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ENotePrioType Prio;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bResolved = false;
};
