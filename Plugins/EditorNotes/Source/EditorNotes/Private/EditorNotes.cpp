#include "EditorNotes.h"
#include "EditorNotesStyle.h"
#include "EditorNotesSettings.h"
#include "EditorNotesCommands.h"
#include "LevelEditor.h"
#include "ISettingsModule.h"
#include "ISettingsSection.h"
#include "ISettingsContainer.h"
#include "Widgets/Docking/SDockTab.h"

namespace
{
	const FName EditorNotesTabName("EditorNotes");
}

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
		//ISettingsContainerPtr SettingsContainer = SettingsModule->GetContainer("Project");
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
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(SEditorNotesWindowWidget)
		];
}

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
