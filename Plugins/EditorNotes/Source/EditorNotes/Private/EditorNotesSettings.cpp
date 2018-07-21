#include "EditorNotesSettings.h"
#include "EditorNoteActor.h"

UEditorNotesSettings::UEditorNotesSettings(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

TSubclassOf<AEditorNoteActor>  UEditorNotesSettings::GetEditorNoteActorClass()
{
	UEditorNotesSettings* EditorNotesSettings = Cast<UEditorNotesSettings>(UEditorNotesSettings::StaticClass()->GetDefaultObject());
	return EditorNotesSettings->EditorNoteActorClass;
}

FString UEditorNotesSettings::GetNotesLevelName()
{
	UEditorNotesSettings* EditorNotesSettings = Cast<UEditorNotesSettings>(UEditorNotesSettings::StaticClass()->GetDefaultObject());
	return EditorNotesSettings->NotesLevelName;
}
