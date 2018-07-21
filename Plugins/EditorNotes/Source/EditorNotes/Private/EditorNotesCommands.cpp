#include "EditorNotesCommands.h"

#define LOCTEXT_NAMESPACE "FEditorNotesModule"

void FEditorNotesCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "EditorNotes", "Bring up EditorNotes window", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
