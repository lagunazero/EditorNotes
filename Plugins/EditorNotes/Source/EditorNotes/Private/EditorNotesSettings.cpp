// Copyright 2015 Moritz Wundke. All Rights Reserved.
// Released under MIT.
 
#include "EditorNotesSettings.h"
#include "EditorNotes.h"
#include "EditorNoteActor.h"

UEditorNotesSettings::UEditorNotesSettings(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
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

const FScrollBoxStyle& UEditorNotesSettings::GetWidgetListStyle()
{
	UEditorNotesSettings* EditorNotesSettings = Cast<UEditorNotesSettings>(UEditorNotesSettings::StaticClass()->GetDefaultObject());
	return EditorNotesSettings->WidgetListStyle;
}
