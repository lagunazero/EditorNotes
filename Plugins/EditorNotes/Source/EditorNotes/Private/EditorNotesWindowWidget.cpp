#include "EditorNotesWindowWidget.h"
#include "EditorNotes.h"
#include "EditorNoteActor.h"
#include "EditorNotesSettings.h"
#include "UnrealEd.h"
#include "Engine.h"

namespace
{
	const FName ListHeaderName = "Title";
	const FName ListHeaderDate = "Date";
	const FName ListHeaderPrio = "Prio";
	const FName ListHeaderText = "Text";
	const FName ListHeaderResolved = "Resolved";
	const FName ListHeaderLabelArt = "Art";
	const FName ListHeaderLabelCode = "Code";
	const FName ListHeaderLabelDesign = "Design";
	const FName ListHeaderLabelLD = "LD";
	const FName ListHeaderLabelWriting = "Writing";
	const FName ListHeaderLabelAudio = "Audio";
}

// One comparison struct per column in the note list.
struct FCompareItemsBase
{
	EColumnSortMode::Type SortMode = EColumnSortMode::None;

	FCompareItemsBase(EColumnSortMode::Type SortMode)
		: SortMode(SortMode)
	{
	}

	FORCEINLINE bool operator()(const FNoteData::Ptr A, const FNoteData::Ptr B) const
	{
		check(A.IsValid());
		check(B.IsValid());
		check(A->NoteActor.IsValid());
		check(B->NoteActor.IsValid());
		return Compare(A, B);
	}

	virtual bool Compare(const FNoteData::Ptr A, const FNoteData::Ptr B) const
	{
		return true;
	}
};

struct FCompareItemsByTitle : FCompareItemsBase
{
	FCompareItemsByTitle(EColumnSortMode::Type SortMode)
	: FCompareItemsBase(SortMode)
	{
	}

	virtual bool Compare(const FNoteData::Ptr A, const FNoteData::Ptr B) const override
	{
		return SortMode == EColumnSortMode::Descending
			? (A->NoteActor->GetName() > B->NoteActor->GetName())
			: (A->NoteActor->GetName() < B->NoteActor->GetName());
	}
};

struct FCompareItemsByText : FCompareItemsBase
{
	FCompareItemsByText(EColumnSortMode::Type SortMode)
	: FCompareItemsBase(SortMode)
	{
	}

	virtual bool Compare(const FNoteData::Ptr A, const FNoteData::Ptr B) const override
	{
		return SortMode == EColumnSortMode::Descending
			? (A->NoteActor->Text > B->NoteActor->Text)
			: (A->NoteActor->Text < B->NoteActor->Text);
	}
};

struct FCompareItemsByDate : FCompareItemsBase
{
	FCompareItemsByDate(EColumnSortMode::Type SortMode)
	: FCompareItemsBase(SortMode)
	{
	}

	virtual bool Compare(const FNoteData::Ptr A, const FNoteData::Ptr B) const override
	{
		return SortMode == EColumnSortMode::Descending
			? (A->NoteActor->Date > B->NoteActor->Date)
			: (A->NoteActor->Date < B->NoteActor->Date);
	}
};

struct FCompareItemsByPrio : FCompareItemsBase
{
	FCompareItemsByPrio(EColumnSortMode::Type SortMode)
	: FCompareItemsBase(SortMode)
	{
	}

	virtual bool Compare(const FNoteData::Ptr A, const FNoteData::Ptr B) const override
	{
		return SortMode == EColumnSortMode::Descending
			? (A->NoteActor->Prio > B->NoteActor->Prio)
			: (A->NoteActor->Prio < B->NoteActor->Prio);
	}
};

struct FCompareItemsByResolved : FCompareItemsBase
{
	FCompareItemsByResolved(EColumnSortMode::Type SortMode)
	: FCompareItemsBase(SortMode)
	{
	}

	virtual bool Compare(const FNoteData::Ptr A, const FNoteData::Ptr B) const override
	{
		return SortMode == EColumnSortMode::Descending
			? (A->NoteActor->bResolved > B->NoteActor->bResolved)
			: (A->NoteActor->bResolved < B->NoteActor->bResolved);
	}
};

struct FCompareItemsByLabelArt : FCompareItemsBase
{
	FCompareItemsByLabelArt(EColumnSortMode::Type SortMode)
		: FCompareItemsBase(SortMode)
	{
	}

	virtual bool Compare(const FNoteData::Ptr A, const FNoteData::Ptr B) const override
	{
		return SortMode == EColumnSortMode::Descending
			? (A->NoteActor->bArt > B->NoteActor->bArt)
			: (A->NoteActor->bArt < B->NoteActor->bArt);
	}
};

struct FCompareItemsByLabelCode : FCompareItemsBase
{
	FCompareItemsByLabelCode(EColumnSortMode::Type SortMode)
		: FCompareItemsBase(SortMode)
	{
	}

	virtual bool Compare(const FNoteData::Ptr A, const FNoteData::Ptr B) const override
	{
		return SortMode == EColumnSortMode::Descending
			? (A->NoteActor->bCode > B->NoteActor->bCode)
			: (A->NoteActor->bCode < B->NoteActor->bCode);
	}
};

struct FCompareItemsByLabelDesign : FCompareItemsBase
{
	FCompareItemsByLabelDesign(EColumnSortMode::Type SortMode)
		: FCompareItemsBase(SortMode)
	{
	}

	virtual bool Compare(const FNoteData::Ptr A, const FNoteData::Ptr B) const override
	{
		return SortMode == EColumnSortMode::Descending
			? (A->NoteActor->bDesign > B->NoteActor->bDesign)
			: (A->NoteActor->bDesign < B->NoteActor->bDesign);
	}
};

struct FCompareItemsByLabelLD : FCompareItemsBase
{
	FCompareItemsByLabelLD(EColumnSortMode::Type SortMode)
		: FCompareItemsBase(SortMode)
	{
	}

	virtual bool Compare(const FNoteData::Ptr A, const FNoteData::Ptr B) const override
	{
		return SortMode == EColumnSortMode::Descending
			? (A->NoteActor->bLD > B->NoteActor->bLD)
			: (A->NoteActor->bLD < B->NoteActor->bLD);
	}
};

struct FCompareItemsByLabelWriting : FCompareItemsBase
{
	FCompareItemsByLabelWriting(EColumnSortMode::Type SortMode)
		: FCompareItemsBase(SortMode)
	{
	}

	virtual bool Compare(const FNoteData::Ptr A, const FNoteData::Ptr B) const override
	{
		return SortMode == EColumnSortMode::Descending
			? (A->NoteActor->bWriting > B->NoteActor->bWriting)
			: (A->NoteActor->bWriting < B->NoteActor->bWriting);
	}
};

struct FCompareItemsByLabelAudio : FCompareItemsBase
{
	FCompareItemsByLabelAudio(EColumnSortMode::Type SortMode)
		: FCompareItemsBase(SortMode)
	{
	}

	virtual bool Compare(const FNoteData::Ptr A, const FNoteData::Ptr B) const override
	{
		return SortMode == EColumnSortMode::Descending
			? (A->NoteActor->bAudio > B->NoteActor->bAudio)
			: (A->NoteActor->bAudio < B->NoteActor->bAudio);
	}
};

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SEditorNotesWindowWidget::Construct(const FArguments& args)
{
#if WITH_EDITOR

	// Create the window widget with all of its contents.
	ChildSlot
	[
		SNew(SBox)
		//.HAlign(EHorizontalAlignment::HAlign_Fill)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.VAlign(EVerticalAlignment::VAlign_Top)
			.HAlign(EHorizontalAlignment::HAlign_Fill)
			.AutoHeight()
			[
				// Buttons
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.HAlign(EHorizontalAlignment::HAlign_Left)
				[
					SNew(SButton)
					.Text(FText::FromString("Add Note"))
					.OnClicked(this, &SEditorNotesWindowWidget::OnAddItemButton)
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.HAlign(EHorizontalAlignment::HAlign_Left)
				[
					SNew(SButton)
					.Text(FText::FromString("Refresh"))
					.OnClicked(this, &SEditorNotesWindowWidget::Refresh)
				]
			]
			+ SVerticalBox::Slot()
			.VAlign(EVerticalAlignment::VAlign_Fill)
			.HAlign(EHorizontalAlignment::HAlign_Fill)
			[
				// Note list
				SNew(SBox)
				.MinDesiredWidth(1000)
				.MinDesiredHeight(1000)
				.HAlign(EHorizontalAlignment::HAlign_Fill)
				[
					SNew(SScrollBox)
					+ SScrollBox::Slot()
					[
						SAssignNew(ListViewWidget, SListView<FNoteData::Ptr>)
						.ItemHeight(24)
						.ListItemsSource(&Items)
						.SelectionMode(ESelectionMode::Single)
						.OnSelectionChanged(this, &SEditorNotesWindowWidget::OnNoteItemSelected)
						//.OnMouseButtonClick
						.OnGenerateRow(this, &SEditorNotesWindowWidget::OnGenerateRowForList)
						.HeaderRow
						(
							SNew(SHeaderRow)

							+ SHeaderRow::Column(ListHeaderName)
							.DefaultLabel(FText::FromName(ListHeaderName))
							.SortMode(TitleSortMode)
							.OnSort(this, &SEditorNotesWindowWidget::OnRowSorted)

							+ SHeaderRow::Column(ListHeaderText)
							.DefaultLabel(FText::FromName(ListHeaderText))
							.SortMode(TextSortMode)
							.OnSort(this, &SEditorNotesWindowWidget::OnRowSorted)

							+SHeaderRow::Column(ListHeaderDate)
							.DefaultLabel(FText::FromName(ListHeaderDate))
							.SortMode(DateSortMode)
							.OnSort(this, &SEditorNotesWindowWidget::OnRowSorted)

							+ SHeaderRow::Column(ListHeaderPrio)
							.DefaultLabel(FText::FromName(ListHeaderPrio))
							.SortMode(PrioSortMode)
							.OnSort(this, &SEditorNotesWindowWidget::OnRowSorted)

							+ SHeaderRow::Column(ListHeaderResolved)
							.DefaultLabel(FText::FromName(ListHeaderResolved))
							.SortMode(ResolvedSortMode)
							.OnSort(this, &SEditorNotesWindowWidget::OnRowSorted)

							+ SHeaderRow::Column(ListHeaderLabelArt)
							.DefaultLabel(FText::FromName(ListHeaderLabelArt))
							.SortMode(LabelArtSortMode)
							.OnSort(this, &SEditorNotesWindowWidget::OnRowSorted)

							+ SHeaderRow::Column(ListHeaderLabelCode)
							.DefaultLabel(FText::FromName(ListHeaderLabelCode))
							.SortMode(LabelCodeSortMode)
							.OnSort(this, &SEditorNotesWindowWidget::OnRowSorted)

							+ SHeaderRow::Column(ListHeaderLabelDesign)
							.DefaultLabel(FText::FromName(ListHeaderLabelDesign))
							.SortMode(LabelDesignSortMode)
							.OnSort(this, &SEditorNotesWindowWidget::OnRowSorted)

							+ SHeaderRow::Column(ListHeaderLabelLD)
							.DefaultLabel(FText::FromName(ListHeaderLabelLD))
							.SortMode(LabelLDSortMode)
							.OnSort(this, &SEditorNotesWindowWidget::OnRowSorted)

							+ SHeaderRow::Column(ListHeaderLabelWriting)
							.DefaultLabel(FText::FromName(ListHeaderLabelWriting))
							.SortMode(LabelWritingSortMode)
							.OnSort(this, &SEditorNotesWindowWidget::OnRowSorted)

							+ SHeaderRow::Column(ListHeaderLabelAudio)
							.DefaultLabel(FText::FromName(ListHeaderLabelAudio))
							.SortMode(LabelAudioSortMode)
							.OnSort(this, &SEditorNotesWindowWidget::OnRowSorted)
						)
					]
				]
			]
		]
	];

	Refresh();
#endif
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

TSharedRef<ITableRow> SEditorNotesWindowWidget::OnGenerateRowForList(FNoteData::Ptr Item, const TSharedRef<STableViewBase>& OwnerTable)
{
	// A class for the row's window widget. Could be in the header file, but only used here.
	class SEditorNoteRowWidget : public SMultiColumnTableRow<FNoteData::Ptr >
	{
	public:
		SLATE_BEGIN_ARGS(SEditorNoteRowWidget){}
		SLATE_END_ARGS()

		void Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTable, FNoteData::Ptr InItem)
		{
			Item = InItem;
			SMultiColumnTableRow< FNoteData::Ptr >::Construct(FSuperRowType::FArguments(), InOwnerTable);
		}

		TSharedRef<SWidget> GenerateWidgetForColumn(const FName& ColumnName)
		{
			// Use the column to set the content.
			FString ItemText = "";
			if (ColumnName.IsEqual(ListHeaderName))
			{
				ItemText = Item->NoteActor->Title;
				if (ItemText.IsEmpty())
				{
					ItemText = Item->NoteActor->GetName();
				}
			}
			else if (ColumnName.IsEqual(ListHeaderText))
			{
				ItemText = Item->NoteActor->Text;
			}
			else if (ColumnName.IsEqual(ListHeaderDate))
			{
				ItemText = Item->NoteActor->Date.ToString();
			}
			else if (ColumnName.IsEqual(ListHeaderPrio))
			{
				ItemText = Item->NoteActor->GetPrioAsString();
			}
			else if (ColumnName.IsEqual(ListHeaderResolved))
			{
				ItemText = Item->NoteActor->bResolved ? "X" : FString();
			}
			else if (ColumnName.IsEqual(ListHeaderLabelArt))
			{
				ItemText = Item->NoteActor->bArt ? "X" : FString();
			}
			else if (ColumnName.IsEqual(ListHeaderLabelCode))
			{
				ItemText = Item->NoteActor->bCode ? "X" : FString();
			}
			else if (ColumnName.IsEqual(ListHeaderLabelDesign))
			{
				ItemText = Item->NoteActor->bDesign ? "X" : FString();
			}
			else if (ColumnName.IsEqual(ListHeaderLabelLD))
			{
				ItemText = Item->NoteActor->bLD ? "X" : FString();
			}
			else if (ColumnName.IsEqual(ListHeaderLabelWriting))
			{
				ItemText = Item->NoteActor->bWriting ? "X" : FString();
			}
			else if (ColumnName.IsEqual(ListHeaderLabelAudio))
			{
				ItemText = Item->NoteActor->bAudio ? "X" : FString();
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Unhandled column name: %s"), *ColumnName.ToString());
			}

			return SNew(STextBlock)
				.Text(FText::FromString(ItemText));
		}
		FNoteData::Ptr Item;
	};

	return SNew(SEditorNoteRowWidget, OwnerTable, Item);
}

void SEditorNotesWindowWidget::OnRowSorted(EColumnSortPriority::Type SortPrio, const FName& ColumnName, EColumnSortMode::Type SortMode)
{
	UE_LOG(LogTemp, Log, TEXT("Sorting by: %s"), *ColumnName.ToString());
	
	// Flip ascending/descending order depending on the most recent order in the clicked column.
	if (ColumnName.IsEqual(ListHeaderName))
	{
		EColumnSortMode::Type NewSortMode = (TitleSortMode.Get() == EColumnSortMode::Ascending)
			? EColumnSortMode::Descending
			: EColumnSortMode::Ascending;
		TitleSortMode.Set(NewSortMode);
		Items.Sort(FCompareItemsByTitle(NewSortMode));
	}
	else if (ColumnName.IsEqual(ListHeaderText))
	{
		EColumnSortMode::Type NewSortMode = (TextSortMode.Get() == EColumnSortMode::Ascending)
			? EColumnSortMode::Descending
			: EColumnSortMode::Ascending;
		TextSortMode.Set(NewSortMode);
		Items.Sort(FCompareItemsByText(NewSortMode));
	}
	else if (ColumnName.IsEqual(ListHeaderDate))
	{
		EColumnSortMode::Type NewSortMode = (DateSortMode.Get() == EColumnSortMode::Ascending)
			? EColumnSortMode::Descending
			: EColumnSortMode::Ascending;
		DateSortMode.Set(NewSortMode);
		Items.Sort(FCompareItemsByDate(NewSortMode));
	}
	else if (ColumnName.IsEqual(ListHeaderPrio))
	{
		EColumnSortMode::Type NewSortMode = (PrioSortMode.Get() == EColumnSortMode::Ascending)
			? EColumnSortMode::Descending
			: EColumnSortMode::Ascending;
		PrioSortMode.Set(NewSortMode);
		Items.Sort(FCompareItemsByPrio(NewSortMode));
	}
	else if (ColumnName.IsEqual(ListHeaderResolved))
	{
		EColumnSortMode::Type NewSortMode = (ResolvedSortMode.Get() == EColumnSortMode::Ascending)
			? EColumnSortMode::Descending
			: EColumnSortMode::Ascending;
		ResolvedSortMode.Set(NewSortMode);
		Items.Sort(FCompareItemsByResolved(NewSortMode));
	}
	else if (ColumnName.IsEqual(ListHeaderLabelArt))
	{
		EColumnSortMode::Type NewSortMode = (LabelArtSortMode.Get() == EColumnSortMode::Ascending)
			? EColumnSortMode::Descending
			: EColumnSortMode::Ascending;
		LabelArtSortMode.Set(NewSortMode);
		Items.Sort(FCompareItemsByLabelArt(NewSortMode));
	}
	else if (ColumnName.IsEqual(ListHeaderLabelCode))
	{
		EColumnSortMode::Type NewSortMode = (LabelCodeSortMode.Get() == EColumnSortMode::Ascending)
			? EColumnSortMode::Descending
			: EColumnSortMode::Ascending;
		LabelCodeSortMode.Set(NewSortMode);
		Items.Sort(FCompareItemsByLabelCode(NewSortMode));
	}
	else if (ColumnName.IsEqual(ListHeaderLabelDesign))
	{
		EColumnSortMode::Type NewSortMode = (LabelDesignSortMode.Get() == EColumnSortMode::Ascending)
			? EColumnSortMode::Descending
			: EColumnSortMode::Ascending;
		LabelDesignSortMode.Set(NewSortMode);
		Items.Sort(FCompareItemsByLabelDesign(NewSortMode));
	}
	else if (ColumnName.IsEqual(ListHeaderLabelLD))
	{
		EColumnSortMode::Type NewSortMode = (LabelLDSortMode.Get() == EColumnSortMode::Ascending)
			? EColumnSortMode::Descending
			: EColumnSortMode::Ascending;
		LabelLDSortMode.Set(NewSortMode);
		Items.Sort(FCompareItemsByLabelLD(NewSortMode));
	}
	else if (ColumnName.IsEqual(ListHeaderLabelWriting))
	{
		EColumnSortMode::Type NewSortMode = (LabelWritingSortMode.Get() == EColumnSortMode::Ascending)
			? EColumnSortMode::Descending
			: EColumnSortMode::Ascending;
		LabelWritingSortMode.Set(NewSortMode);
		Items.Sort(FCompareItemsByLabelWriting(NewSortMode));
	}
	else if (ColumnName.IsEqual(ListHeaderLabelAudio))
	{
		EColumnSortMode::Type NewSortMode = (LabelAudioSortMode.Get() == EColumnSortMode::Ascending)
			? EColumnSortMode::Descending
			: EColumnSortMode::Ascending;
		LabelAudioSortMode.Set(NewSortMode);
		Items.Sort(FCompareItemsByLabelAudio(NewSortMode));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Unhandled column name: %s"), *ColumnName.ToString());
	}

	ListViewWidget->RequestListRefresh();
}

void SEditorNotesWindowWidget::OnNoteItemSelected(FNoteData::Ptr Item, ESelectInfo::Type SelectInfo)
{
#if WITH_EDITOR
	if (!Item.IsValid() || !Item->NoteActor.IsValid()) return;

	TWeakObjectPtr<AEditorNoteActor> Actor = Item->NoteActor;
	UE_LOG(LogTemp, Log, TEXT("Selected %s"), *Actor->GetName());

	// Select note actor in editor.
	if (GEditor)
	{
		GEditor->SelectNone(false, true);
		GEditor->SelectActor(Actor.Get(), true, true, true, true);
		GEditor->MoveViewportCamerasToActor(*Actor.Get(), true);
	}
#endif
}

FReply SEditorNotesWindowWidget::Refresh()
{
#if WITH_EDITOR
	UE_LOG(LogTemp, Log, TEXT("Refreshing..."));

	UWorld* World = GetEditorWorld();
	if (!World)
	{
		return FReply::Handled();
	}

	// Clear and reset list.
	Items.Empty();
	for (TActorIterator<AEditorNoteActor> Itr(World); Itr; ++Itr)
	{
		if (AEditorNoteActor* Actor = *Itr)
		{
			auto Item = MakeShareable(new FNoteData());
			Item.Object->NoteActor = Actor;
			Items.Add(Item);
		}
	}

	//Update the listview.
	ListViewWidget->RequestListRefresh();
	UE_LOG(LogTemp, Log, TEXT("Refresh done."));
#endif
	return FReply::Handled();
}

FReply SEditorNotesWindowWidget::OnAddItemButton()
{
#if WITH_EDITOR
	UE_LOG(LogTemp, Log, TEXT("Adding Note..."));
	UWorld* World = GetEditorWorld();
	if (!World)
	{
		return FReply::Handled();
	}

	TSubclassOf<AEditorNoteActor> EditorNoteActorClass = UEditorNotesSettings::GetEditorNoteActorClass();
	if (EditorNoteActorClass.Get())
	{
		FString NotesLevelName = UEditorNotesSettings::GetNotesLevelName();
		if (!NotesLevelName.IsEmpty())
		{
			const FString PersistentLevelName = UGameplayStatics::GetCurrentLevelName(World);
			//UE_LOG(LogTemp, Log, TEXT("Persistent level name: %s"), *PersistentLevelName);
			NotesLevelName = PersistentLevelName + NotesLevelName;
			UE_LOG(LogTemp, Log, TEXT("Adding Note to level: %s"), *NotesLevelName);
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Adding Note to persistent level."));
		}

		// Create Note actor, then select it. Don't focus to it cause that would be annoying.
		if (AEditorNoteActor* SpawnedNote = Cast<AEditorNoteActor>(SpawnActorIntoLevel(EditorNoteActorClass, NotesLevelName)))
		{
			SpawnedNote->InitNote();
			Refresh();
			GEditor->SelectNone(false, true);
			GEditor->SelectActor(SpawnedNote, true, true, true, true);
			UE_LOG(LogTemp, Log, TEXT("Note created."));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to create Note."));
			return FReply::Handled();
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("EditorNoteActor has invalid/missing type."));
		return FReply::Handled();
	}
#endif
	return FReply::Handled();
}

AActor* SEditorNotesWindowWidget::SpawnActorIntoLevel(TSubclassOf<AActor> ActorClass, FString LevelName, FVector Location, FRotator Rotation)
{
	if (!ActorClass) return nullptr;

	UWorld* const World = GetEditorWorld();
	if (!World) return nullptr;

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParameters.bDeferConstruction = false;

	//Get Level from Name
	ULevel* FoundLevel = nullptr;

	if (!LevelName.IsEmpty())
	{
		//UE_LOG(LogTemp, Log, TEXT("Checking %d streaming levels..."), World->GetStreamingLevels().Num());
		for (const ULevelStreaming* EachLevel : World->GetStreamingLevels())
		{
			if (!EachLevel) continue;
			//UE_LOG(LogTemp, Log, TEXT("ULevelStreaming: %s"), *EachLevel->GetName());
			ULevel* LevelPtr = EachLevel->GetLoadedLevel();
			//UE_LOG(LogTemp, Log, TEXT("LevelPtr: %s"), *LevelPtr->GetName());
			if (!LevelPtr) continue;

			const FString PackageName = EachLevel->GetWorldAssetPackageName();
			TArray<FString> Tokens;
			const TCHAR delim = '/'; // TODO: Make platform independent
			PackageName.ParseIntoArray(Tokens, &delim);
			//UE_LOG(LogTemp, Log, TEXT("PackageName: %s"), *PackageName);
			//UE_LOG(LogTemp, Log, TEXT("Tokens: %d"), Tokens.Num());
			//if (Tokens.Num() > 0)
			//{
			//	UE_LOG(LogTemp, Log, TEXT("Token: %s"), *Tokens.Last());
			//}
			if (Tokens.Num() > 0 && Tokens.Last().Equals(LevelName))
			{
				FoundLevel = LevelPtr;
				break;
			}
		}
	}
	else
	{
		FoundLevel = World->PersistentLevel;
	}

	if (!FoundLevel)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to spawn actor in level \"%s\". Level not found."), *LevelName);
		return nullptr;
	}

	SpawnParameters.OverrideLevel = FoundLevel;
	return World->SpawnActor(ActorClass, &Location, &Rotation, SpawnParameters);
}

UWorld* SEditorNotesWindowWidget::GetEditorWorld()
{
	if (!GEditor)
	{
		UE_LOG(LogTemp, Warning, TEXT("No GEditor."));
		return nullptr;
	}

	FWorldContext& WorldContext = GEditor->GetEditorWorldContext();
	UWorld* World = WorldContext.World();
	if (!WorldContext.World())
	{
		UE_LOG(LogTemp, Warning, TEXT("No GEditor WorldContext."));
		return nullptr;
	}
	return World;
}
