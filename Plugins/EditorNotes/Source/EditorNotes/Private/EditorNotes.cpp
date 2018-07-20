// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "EditorNotes.h"
#include "EditorNotesStyle.h"
#include "EditorNotesSettings.h"
#include "EditorNotesCommands.h"
#include "LevelEditor.h"
#include "ISettingsModule.h"
#include "ISettingsSection.h"
#include "ISettingsContainer.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Widgets/Views/SHeaderRow.h"

static const FName EditorNotesTabName("EditorNotes");

#define LOCTEXT_NAMESPACE "FEditorNotesModule"

void FEditorNotesModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FEditorNotesStyle::Initialize();
	FEditorNotesStyle::ReloadTextures();

	FEditorNotesCommands::Register();

	RegisterSettings();

	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FEditorNotesCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FEditorNotesModule::PluginButtonClicked),
		FCanExecuteAction());
		
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	
	{
		TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
		MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, PluginCommands, FMenuExtensionDelegate::CreateRaw(this, &FEditorNotesModule::AddMenuExtension));

		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}
	
	{
		TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
		ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, PluginCommands, FToolBarExtensionDelegate::CreateRaw(this, &FEditorNotesModule::AddToolbarExtension));
		
		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	}
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(EditorNotesTabName, FOnSpawnTab::CreateRaw(this, &FEditorNotesModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FEditorNotesTabTitle", "EditorNotes"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FEditorNotesModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FEditorNotesStyle::Shutdown();

	FEditorNotesCommands::Unregister();
	
	UnregisterSettings();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(EditorNotesTabName);
}

// Callback for when the settings were saved.
bool FEditorNotesModule::HandleSettingsSaved()
{
	UEditorNotesSettings* Settings = GetMutableDefault<UEditorNotesSettings>();
	bool ResaveSettings = false;

	// You can put any validation code in here and resave the settings in case an invalid
	// value has been entered

	if (ResaveSettings)
	{
		Settings->SaveConfig();
	}

	return true;
}

void FEditorNotesModule::RegisterSettings()
{
	// Registering some settings is just a matter of exposing the default UObject of
	// your desired class, feel free to add here all those settings you want to expose
	// to your LDs or artists.

	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		// Create the new category
		ISettingsContainerPtr SettingsContainer = SettingsModule->GetContainer("Project");

		//SettingsContainer->DescribeCategory("CustomSettings",
		//	LOCTEXT("RuntimeWDCategoryName", "CustomSettings"),
		//	LOCTEXT("RuntimeWDCategoryDescription", "Game configuration for the CustomSettings game module"));

		// Register the settings
		ISettingsSectionPtr SettingsSection = SettingsModule->RegisterSettings("Project", "Plugins", "EditorNotes",
			LOCTEXT("RuntimeGeneralSettingsName", "EditorNotes"),
			LOCTEXT("RuntimeGeneralSettingsDescription", "Base configuration for the EditorNotes"),
			GetMutableDefault<UEditorNotesSettings>()
		);

		// Register the save handler to your settings, you might want to use it to
		// validate those or just act to settings changes.
		if (SettingsSection.IsValid())
		{
			SettingsSection->OnModified().BindRaw(this, &FEditorNotesModule::HandleSettingsSaved);
		}
	}
}

void FEditorNotesModule::UnregisterSettings()
{
	// Ensure to unregister all of your registered settings here, hot-reload would
	// otherwise yield unexpected results.

	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "CustomSettings", "General");
	}
}

TSharedRef<SDockTab> FEditorNotesModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FEditorNotesModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("EditorNotes.cpp"))
		);

	//TArray< TSharedPtr<FString> > Items;


	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(SEditorNotesWindowWidget)
			// Put your tab content here!
			//SNew(SBox)
			//.HAlign(HAlign_Fill)
			//.VAlign(VAlign_Fill)
			//[
			//	//SNew(SListView< TSharedPtr<FString> >)
			//	//.ItemHeight(24)
			//	//.ListItemsSource(&Items)
			//	//.OnGenerateRow(this, &FEditorNotesModule::OnGenerateRowForList)

			//	SNew(SEditorNotesWindowWidget)

				//SNew(STextBlock)
				//.Text(WidgetText)

		/*
				SNew(SListView< TSharedPtr<FString> >)
				.ItemHeight(24)
				.ListItemsSource(&Items)
				.OnGenerateRow(SListView< TSharedPtr<FString> >::GenerateNewWidget( this, &FEditorNotesModule::OnGenerateWidgetForList))
				//.OnContextMenuOpening(this, &FEditorNotesModule::GetListContextMenu)
				//.SelectionMode(this, &FEditorNotesModule::GetSelectionMode)
				.HeaderRow
				(
					SNew(SHeaderRow)
					+ SHeaderRow::Column("Name")
					[
						SNew(SBorder)
						.Padding(5)
					.Content()
					[
						SNew(STextBlock)
						.Text(TEXT("Name"))
					]
					]
			+ SHeaderRow::Column("Number").DefaultLabel(TEXT("Number"))
				+ SHeaderRow::Column("TextField").DefaultLabel(TEXT("Text Field"))
				+ SHeaderRow::Column("TextBlock").DefaultLabel(TEXT("Text Block"))
				+ SHeaderRow::Column("AddChild").DefaultLabel(TEXT("Add Child"))
				)
				*/
			//]
		];
}

//TSharedRef<ITableRow> FEditorNotesModule::OnGenerateRowForList(TSharedPtr<FString> Item, const TSharedRef<STableViewBase>& OwnerTable)
//{
//	//Create the row
//	return
//		SNew(STableRow< TSharedPtr<FString> >, OwnerTable)
//		.Padding(2.0f)
//		[
//			//SNew(STextBlock).Text(FText::FromString(*Item.Get()))
//			SNew(STextBlock).Text(MakeShareable(new FText(FText::FromString("TEST"))))
//		];
//}

void FEditorNotesModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->InvokeTab(EditorNotesTabName);
}

void FEditorNotesModule::AddMenuExtension(FMenuBuilder& Builder)
{
	Builder.AddMenuEntry(FEditorNotesCommands::Get().OpenPluginWindow);
}

void FEditorNotesModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(FEditorNotesCommands::Get().OpenPluginWindow);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FEditorNotesModule, EditorNotes)