#pragma once

#include "SlateBasics.h"

class AEditorNoteActor;

struct FNoteData
{
	TWeakObjectPtr<AEditorNoteActor> NoteActor;
};

struct FNoteRow
{
	FString Name;
	FString Date;
	FString Prio;
	FString Resolved;
};

// Lays out and controls the Main Menu UI for our tutorial.
class /*EDITORNOTES_API*/ SEditorNotesWindowWidget : public SCompoundWidget
{
 
public:
	SLATE_BEGIN_ARGS(SEditorNotesWindowWidget)
	{}
	// SLATE_ARGUMENT(TWeakObjectPtr<class AMainMenuHUD>, MainMenuHUD)
	SLATE_END_ARGS()
 
        // Constructs and lays out the Main Menu UI Widget.
        // args Arguments structure that contains widget-specific setup information.
	void Construct(const FArguments& args);
 
	// Click handler for the Play Game! button – Calls MenuHUD’s PlayGameClicked() event.
	FReply OnAddItemButton();
	FReply Refresh();

	void OnNoteItemSelected(TSharedPtr<FNoteData> Item, ESelectInfo::Type SelectInfo);
	EColumnSortMode::Type GetColumnSortMode(const FName& ColumnId) const;
	void OnRowSorted(EColumnSortPriority::Type SortPrioType, const FName& ColumnName, EColumnSortMode::Type SortModeType);

	// Click handler for the Quit Game button – Calls MenuHUD’s QuitGameClicked() event.
	FReply QuitGameClicked();
 
	TSharedRef<ITableRow> OnGenerateRowForList(TSharedPtr<FNoteData> Item, const TSharedRef<STableViewBase>& OwnerTable);

	TArray<TSharedPtr<FNoteData>> Items;
	TSharedPtr< SListView< TSharedPtr<FNoteData> > > ListViewWidget;

	TAttribute<EColumnSortMode::Type> NameSortMode;
	TAttribute<EColumnSortMode::Type> TextSortMode;
	TAttribute<EColumnSortMode::Type> DateSortMode;
	TAttribute<EColumnSortMode::Type> PrioSortMode;
	TAttribute<EColumnSortMode::Type> ResolvedSortMode;

        // Stores a weak reference to the HUD controlling this class.
	// TWeakObjectPtr<class AMainMenuHUD> MainMenuHUD;
};