#pragma once

#include "SlateBasics.h"

class AEditorNoteActor;

struct FNoteData
{
	TWeakObjectPtr<AEditorNoteActor> NoteActor;
	
	typedef TSharedPtr<FNoteData> Ptr;
};

/**
 * Handles the window widget and all interaction with it.
 */
class /*EDITORNOTES_API*/ SEditorNotesWindowWidget : public SCompoundWidget
{
 
public:
	SLATE_BEGIN_ARGS(SEditorNotesWindowWidget)
	{}
	SLATE_END_ARGS()

	void Construct(const FArguments& args);
 
protected:

	// List functions.
	TSharedRef<ITableRow> OnGenerateRowForList(FNoteData::Ptr Item, const TSharedRef<STableViewBase>& OwnerTable);
	void OnRowSorted(EColumnSortPriority::Type SortPrioType, const FName& ColumnName, EColumnSortMode::Type SortModeType);
	void OnNoteItemSelected(FNoteData::Ptr Item, ESelectInfo::Type SelectInfo);

	// Button functions.
	FReply Refresh();
	FReply OnAddItemButton();

	// Helper functions.
	AActor* SpawnActorIntoLevel(TSubclassOf<AActor> ActorClass, FString LevelName = "", FVector Location = FVector::ZeroVector, FRotator Rotation = FRotator::ZeroRotator);
	UWorld* GetEditorWorld();

	TArray<FNoteData::Ptr> Items;
	TSharedPtr< SListView<FNoteData::Ptr> > ListViewWidget;

	// TODO: Storing/using sorting order like this doesn't work quite the way I want it to.
	// The arrows showing sort direction don't show.
	// The current implementation also doesn't handle "primary" and "secondary" column wrt sorting.
	TAttribute<EColumnSortMode::Type> TitleSortMode;
	TAttribute<EColumnSortMode::Type> TextSortMode;
	TAttribute<EColumnSortMode::Type> DateSortMode;
	TAttribute<EColumnSortMode::Type> PrioSortMode;
	TAttribute<EColumnSortMode::Type> ResolvedSortMode;
};