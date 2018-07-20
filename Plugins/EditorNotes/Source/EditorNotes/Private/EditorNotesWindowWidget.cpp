#include "EditorNotesWindowWidget.h"
#include "EditorNotes.h"
#include "EditorNoteActor.h"
#include "EditorNotesSettings.h"
#include "UnrealEd.h"
#include "EditorNotesFunctionLibrary.h"
#include "Engine.h"
 
BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SEditorNotesWindowWidget::Construct(const FArguments& args)
{
#if WITH_EDITOR

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
						SAssignNew(ListViewWidget, SListView<TSharedPtr<FNoteData>>)
						.ItemHeight(24)
						.ListItemsSource(&Items) //The Items array is the source of this listview
						.SelectionMode(ESelectionMode::Single)
						.OnSelectionChanged(this, &SEditorNotesWindowWidget::OnNoteItemSelected)
						//.OnMouseButtonClick
						.OnGenerateRow(this, &SEditorNotesWindowWidget::OnGenerateRowForList)
						.HeaderRow
						(
							SNew(SHeaderRow)
							+ SHeaderRow::Column("Name")
							.DefaultLabel(FText::FromString("Name"))
							.SortMode(NameSortMode)
							.OnSort(this, &SEditorNotesWindowWidget::OnRowSorted)
							+ SHeaderRow::Column("Text")
							.DefaultLabel(FText::FromString("Text"))
							.SortMode(TextSortMode)
							.OnSort(this, &SEditorNotesWindowWidget::OnRowSorted)
							+SHeaderRow::Column("Date")
							.DefaultLabel(FText::FromString("Date"))
							.SortMode(DateSortMode)
							.OnSort(this, &SEditorNotesWindowWidget::OnRowSorted)
							+ SHeaderRow::Column("Prio")
							.DefaultLabel(FText::FromString("Prio"))
							.SortMode(PrioSortMode)
							.OnSort(this, &SEditorNotesWindowWidget::OnRowSorted)
							+ SHeaderRow::Column("Resolved")
							.DefaultLabel(FText::FromString("Resolved"))
							.SortMode(ResolvedSortMode)
							.OnSort(this, &SEditorNotesWindowWidget::OnRowSorted)
						)
					]
				]
			]
		]
	];


	/*
	TArray< TSharedPtr<FString> > Items;
 
	ChildSlot
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Top)
			[
				SNew(STextBlock)
				.ColorAndOpacity(FLinearColor::White)
				.ShadowColorAndOpacity(FLinearColor::Black)
				.ShadowOffset(FIntPoint(-1, 1))
				.Font(FSlateFontInfo("Arial", 26))
				.Text(FText::FromString("Main Menu"))
			]
			+ SOverlay::Slot()
			.HAlign(HAlign_Right)
			.VAlign(VAlign_Bottom)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				[
					SNew(SButton)
					.Text(FText::FromString("Add note"))
					.OnClicked(this, &SEditorNotesWindowWidget::PlayGameClicked)
				]

				//+ SVerticalBox::Slot()
				//[
				//	SNew(SListView< TSharedPtr<FString> >)
				//	.ItemHeight(24)
				//	.ListItemsSource(&Items)
				//	.OnGenerateRow(this, &SEditorNotesWindowWidget::OnGenerateRowForList)
				//]

				//+ SVerticalBox::Slot()
				//[
				//	SNew(SListView< TSharedPtr<FString> >)
				//	.ItemHeight(24)
				//	.ListItemsSource(&Items)
				//	//.OnGenerateRow(SListView< TSharedPtr<FString> >::GenerateNewWidget(this, &SEditorNotesWindowWidget::OnGenerateWidgetForList))
				//	.OnGenerateRow(this, &SEditorNotesWindowWidget::OnGenerateWidgetForList)
				//	//.OnContextMenuOpening(this, &FEditorNotesModule::GetListContextMenu)
				//	//.SelectionMode(this, &FEditorNotesModule::GetSelectionMode)
				//	.HeaderRow
				//	(
				//		SNew(SHeaderRow)
				//		+ SHeaderRow::Column("Name")
				//		[
				//			SNew(SBorder)
				//			.Padding(5)
				//			.Content()
				//			[
				//				SNew(STextBlock)
				//				.Text(FText::FromString("Name"))
				//			]
				//		]
				//		+ SHeaderRow::Column("Number").DefaultLabel(FText::FromString("Number"))
				//		+ SHeaderRow::Column("TextField").DefaultLabel(FText::FromString("Text Field"))
				//		+ SHeaderRow::Column("TextBlock").DefaultLabel(FText::FromString("Text Block"))
				//		+ SHeaderRow::Column("AddChild").DefaultLabel(FText::FromString("Add Child"))
				//	)
				//]
			]
		];
 */

	Refresh();
#endif
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

TSharedRef<ITableRow> SEditorNotesWindowWidget::OnGenerateRowForList(TSharedPtr<FNoteData> Item, const TSharedRef<STableViewBase>& OwnerTable)
{
	class SEditorNoteRowWidget : public SMultiColumnTableRow<TSharedPtr<FNoteData> >
	{
	public:
		SLATE_BEGIN_ARGS(SEditorNoteRowWidget){}
		SLATE_END_ARGS()

		void Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTable, TSharedPtr<FNoteData> InItem)
		{
			Item = InItem;
			SMultiColumnTableRow< TSharedPtr<FNoteData> >::Construct(FSuperRowType::FArguments(), InOwnerTable);
		}

		TSharedRef<SWidget> GenerateWidgetForColumn(const FName& ColumnName)
		{
			FString ItemText;
			if (ColumnName == "Name")
			{
				ItemText = Item->NoteActor->GetName();
			}
			else if (ColumnName == "Text")
			{
				ItemText = Item->NoteActor->Text;
			}
			else if (ColumnName == "Date")
			{
				ItemText = Item->NoteActor->Date.ToString();
			}
			else if (ColumnName == "Prio")
			{
				ItemText = Item->NoteActor->PrioAsString();
			}
			else if (ColumnName == "Resolved")
			{
				ItemText = Item->NoteActor->bResolved ? "X" : "";
			}

			return SNew(STextBlock)
				.Text(FText::FromString(ItemText));
				//.Font(FEditorStyle::GetFontStyle(TEXT("ShooterGame.MenuServerListFont")))
				//.ShadowColorAndOpacity(FLinearColor::Black)
				//.ShadowOffset(FIntPoint(-1, 1));
		}
		TSharedPtr<FNoteData> Item;
	};

	return SNew(SEditorNoteRowWidget, OwnerTable, Item);
}

struct FCompareItemsByName
{
	EColumnSortMode::Type SortMode = EColumnSortMode::None;

	FCompareItemsByName(EColumnSortMode::Type SortMode)
		: SortMode(SortMode)
	{
	}

	FORCEINLINE bool operator()(const TSharedPtr<FNoteData> A, const TSharedPtr<FNoteData> B) const
	{
		check(A.IsValid());
		check(B.IsValid());
		check(A->NoteActor.IsValid());
		check(B->NoteActor.IsValid());
		return SortMode == EColumnSortMode::Descending
			? (A->NoteActor->GetName() > B->NoteActor->GetName())
			: (A->NoteActor->GetName() < B->NoteActor->GetName());
	}
};

struct FCompareItemsByText
{
	EColumnSortMode::Type SortMode = EColumnSortMode::None;

	FCompareItemsByText(EColumnSortMode::Type SortMode)
		: SortMode(SortMode)
	{
	}

	FORCEINLINE bool operator()(const TSharedPtr<FNoteData> A, const TSharedPtr<FNoteData> B) const
	{
		check(A.IsValid());
		check(B.IsValid());
		check(A->NoteActor.IsValid());
		check(B->NoteActor.IsValid());
		return SortMode == EColumnSortMode::Descending
			? (A->NoteActor->Text > B->NoteActor->Text)
			: (A->NoteActor->Text < B->NoteActor->Text);
	}
};

struct FCompareItemsByDate
{
	EColumnSortMode::Type SortMode = EColumnSortMode::None;

	FCompareItemsByDate(EColumnSortMode::Type SortMode)
		: SortMode(SortMode)
	{
	}

	FORCEINLINE bool operator()(const TSharedPtr<FNoteData> A, const TSharedPtr<FNoteData> B) const
	{
		check(A.IsValid());
		check(B.IsValid());
		check(A->NoteActor.IsValid());
		check(B->NoteActor.IsValid());
		return SortMode == EColumnSortMode::Descending
			? (A->NoteActor->Date > B->NoteActor->Date)
			: (A->NoteActor->Date < B->NoteActor->Date);
	}
};

struct FCompareItemsByPrio
{
	EColumnSortMode::Type SortMode = EColumnSortMode::None;

	FCompareItemsByPrio(EColumnSortMode::Type SortMode)
		: SortMode(SortMode)
	{
	}

	FORCEINLINE bool operator()(const TSharedPtr<FNoteData> A, const TSharedPtr<FNoteData> B) const
	{
		check(A.IsValid());
		check(B.IsValid());
		check(A->NoteActor.IsValid());
		check(B->NoteActor.IsValid());
		return SortMode == EColumnSortMode::Descending
			? (A->NoteActor->Prio > B->NoteActor->Prio)
			: (A->NoteActor->Prio < B->NoteActor->Prio);
	}
};

struct FCompareItemsByResolved
{
	EColumnSortMode::Type SortMode = EColumnSortMode::None;

	FCompareItemsByResolved(EColumnSortMode::Type SortMode)
		: SortMode(SortMode)
	{
	}

	FORCEINLINE bool operator()(const TSharedPtr<FNoteData> A, const TSharedPtr<FNoteData> B) const
	{
		check(A.IsValid());
		check(B.IsValid());
		check(A->NoteActor.IsValid());
		check(B->NoteActor.IsValid());
		return SortMode == EColumnSortMode::Descending
			? (A->NoteActor->bResolved > B->NoteActor->bResolved)
			: (A->NoteActor->bResolved < B->NoteActor->bResolved);
	}
};

void SEditorNotesWindowWidget::OnRowSorted(EColumnSortPriority::Type SortPrio, const FName& ColumnName, EColumnSortMode::Type SortMode)
{
	UE_LOG(LogTemp, Log, TEXT("Sorting by: %s"), *ColumnName.ToString());
	
	if (ColumnName.IsEqual("Name"))
	{
		EColumnSortMode::Type NewSortMode = (NameSortMode.Get() == EColumnSortMode::Ascending)
			? EColumnSortMode::Descending
			: EColumnSortMode::Ascending;
		NameSortMode.Set(NewSortMode);
		Items.Sort(FCompareItemsByName(NewSortMode));
	}
	else if (ColumnName.IsEqual("Text"))
	{
		EColumnSortMode::Type NewSortMode = (TextSortMode.Get() == EColumnSortMode::Ascending)
			? EColumnSortMode::Descending
			: EColumnSortMode::Ascending;
		TextSortMode.Set(NewSortMode);
		Items.Sort(FCompareItemsByText(NewSortMode));
	}
	else if (ColumnName.IsEqual("Date"))
	{
		EColumnSortMode::Type NewSortMode = (DateSortMode.Get() == EColumnSortMode::Ascending)
			? EColumnSortMode::Descending
			: EColumnSortMode::Ascending;
		DateSortMode.Set(NewSortMode);
		Items.Sort(FCompareItemsByDate(NewSortMode));
	}
	else if (ColumnName.IsEqual("Prio"))
	{
		EColumnSortMode::Type NewSortMode = (PrioSortMode.Get() == EColumnSortMode::Ascending)
			? EColumnSortMode::Descending
			: EColumnSortMode::Ascending;
		PrioSortMode.Set(NewSortMode);
		Items.Sort(FCompareItemsByPrio(NewSortMode));
	}
	else if (ColumnName.IsEqual("Resolved"))
	{
		EColumnSortMode::Type NewSortMode = (ResolvedSortMode.Get() == EColumnSortMode::Ascending)
			? EColumnSortMode::Descending
			: EColumnSortMode::Ascending;
		ResolvedSortMode.Set(NewSortMode);
		Items.Sort(FCompareItemsByResolved(NewSortMode));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Unhandled column name: %s"), *ColumnName.ToString());
	}

	ListViewWidget->RequestListRefresh();
		/*
		if (OldSortMode == EColumnSortMode::Ascending)
		{
			UE_LOG(LogTemp, Log, TEXT("old ascending"));
		}
		else if (OldSortMode == EColumnSortMode::Descending)
		{
			UE_LOG(LogTemp, Log, TEXT("old descending"));
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("old unsorted"));
		}
		if (NewSortMode == EColumnSortMode::Ascending)
		{
			UE_LOG(LogTemp, Log, TEXT("new ascending"));
		}
		else if (NewSortMode == EColumnSortMode::Descending)
		{
			UE_LOG(LogTemp, Log, TEXT("new descending"));
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("new unsorted"));
		}
		*/
}

FReply SEditorNotesWindowWidget::OnAddItemButton()
{
#if WITH_EDITOR
	//Adds a new item to the array (do whatever you want with this)
	//Items.Add(MakeShareable(new FString("Hello 1")));
	//Items.Add(MakeShareable(new FNoteData()));

	if (!GEditor)
	{
		UE_LOG(LogTemp, Log, TEXT("editor world"));
		return FReply::Handled();
	}
	FWorldContext& WorldContext = GEditor->GetEditorWorldContext();
	UWorld* World = WorldContext.World();
	if (!WorldContext.World())
	{
		UE_LOG(LogTemp, Log, TEXT("no world"));
		return FReply::Handled();
	}

	TSubclassOf<AEditorNoteActor> EditorNoteActorClass = UEditorNotesSettings::GetEditorNoteActorClass();
	if (EditorNoteActorClass.Get())
	{
		FString NotesLevelName = UEditorNotesSettings::GetNotesLevelName();
		UE_LOG(LogTemp, Log, TEXT("NotesLevelName: %s"), *NotesLevelName);
		if (!NotesLevelName.IsEmpty())
		{
			const FString PersistentLevelName = UGameplayStatics::GetCurrentLevelName(World);
			UE_LOG(LogTemp, Log, TEXT("PersistentLevelName: %s"), *PersistentLevelName);
			NotesLevelName = PersistentLevelName + NotesLevelName;
		}

		UE_LOG(LogTemp, Log, TEXT("got as subclass"));
		AActor* SpawnedActor = UEditorNotesFunctionLibrary::SpawnActorIntoLevel(World, EditorNoteActorClass, NotesLevelName);
		if (SpawnedActor)
		{
			UE_LOG(LogTemp, Log, TEXT("spawn success"));
			if (AEditorNoteActor* SpawnedNote = Cast<AEditorNoteActor>(SpawnedActor))
			{
				UE_LOG(LogTemp, Log, TEXT("init note"));
				SpawnedNote->InitNote();
				Refresh();
				GEditor->SelectNone(false, true);
				GEditor->SelectActor(SpawnedNote, true, true, true, true);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("no valid subclass"));
	}

	//UClass* EditorNoteClass = nullptr;
	//UObject* EditorNoteObject = LoadObject<UObject>(nullptr, *UEditorNotesSettings::GetEditorNoteActorClass());
	//if (UBlueprint* EditorNoteAsBlueprint = Cast<UBlueprint>(EditorNoteObject))
	//{
	//	UE_LOG(LogTemp, Log, TEXT("got as bp"));
	//	EditorNoteClass = EditorNoteAsBlueprint->GeneratedClass;
	//}
	//if (EditorNoteClass)
	//{
	//	UE_LOG(LogTemp, Log, TEXT("got class"));
	//}

	//if (!GConfig)
	//{
	//	UE_LOG(LogTemp, Log, TEXT("no gconfig"));
	//	return FReply::Handled();
	//}
	//FString ValueReceived;
	//bool r = GConfig->GetString(
	//	TEXT("Plugins.EditorNotes.General"),
	//	TEXT("EditorNoteActorClass"),
	//	ValueReceived,
	//	GGameIni
	//);
	//if (r)
	//{
	//	UE_LOG(LogTemp, Log, TEXT("%s"), *ValueReceived);
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Log, TEXT("fail get"));
	//}
	//return FReply::Handled();

	//static ConstructorHelpers::FObjectFinder<UBlueprint> BpNote(TEXT("Blueprint'/EditorNotes/BP_EditorNoteActor.BP_EditorNoteActor'"));
	//if (BpNote.Object)
	//{
	//	UE_LOG(LogTemp, Log, TEXT("find"));
	//	UEditorNotesFunctionLibrary::SpawnActorIntoLevel(World, BpNote.Object->GetClass());
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Log, TEXT("no find"));
	//}
	
	//Update the listview
	//ListViewWidget->RequestListRefresh();
#endif
	return FReply::Handled();
}

FReply SEditorNotesWindowWidget::Refresh()
{
#if WITH_EDITOR
	UE_LOG(LogTemp, Log, TEXT("refreshing..."));

	if (!GEditor)
	{
		UE_LOG(LogTemp, Log, TEXT("no geditor"));
		return FReply::Handled();
	}

	FWorldContext& WorldContext = GEditor->GetEditorWorldContext();
	UWorld* World = WorldContext.World();
	if (!WorldContext.World())
	{
		UE_LOG(LogTemp, Log, TEXT("no worldcontext world"));
		return FReply::Handled();
	}

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

	UE_LOG(LogTemp, Log, TEXT("refresh done"));
	//Update the listview
	ListViewWidget->RequestListRefresh();
#endif
	return FReply::Handled();
}

void SEditorNotesWindowWidget::OnNoteItemSelected(TSharedPtr<FNoteData> Item, ESelectInfo::Type SelectInfo)
{
#if WITH_EDITOR
	if (!Item.IsValid() || !Item->NoteActor.IsValid()) return;

	TWeakObjectPtr<AEditorNoteActor> Actor = Item->NoteActor;
	UE_LOG(LogTemp, Log, TEXT("selected %s"), *Actor->GetName());

	if (GEditor)
	{
		GEditor->SelectNone(false, true);
		GEditor->SelectActor(Actor.Get(), true, true, true, true);
		GEditor->MoveViewportCamerasToActor(*Actor.Get(), true);
	}
#endif
}

FReply SEditorNotesWindowWidget::QuitGameClicked()
{
#if WITH_EDITOR
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("QuitGameClicked"));
	}
#endif
	return FReply::Handled();
}