#pragma once

#include "CoreMinimal.h"
#include "ModuleManager.h"
#include "Widgets/Views/SListView.h"
#include "EditorNotesWindowWidget.h"

class FToolBarBuilder;
class FMenuBuilder;

/**
 * Main class for the module. Handles startup/shutdown, registering settings and other big picture stuff.
 */
class FEditorNotesModule : public IModuleInterface
{
public:
	SLATE_BEGIN_ARGS(FEditorNotesModule) {}
	SLATE_END_ARGS()

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	virtual bool SupportsDynamicReloading() override
	{
		return true;
	}

private:

	// Settings.
	bool HandleSettingsSaved();
	void RegisterSettings();
	void UnregisterSettings();

	// Startup helper functions.
	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);
	/** This function will be bound to Command (by default it will bring up plugin window) */
	void PluginButtonClicked();
	void AddMenuExtension(FMenuBuilder& Builder);
	void AddToolbarExtension(FToolBarBuilder& Builder);

private:
	TSharedPtr<class FUICommandList> PluginCommands;
};
