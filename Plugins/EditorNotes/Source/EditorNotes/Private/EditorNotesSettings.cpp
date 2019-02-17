#include "EditorNotesSettings.h"
#include "EditorNoteActor.h"
#include "Math/Color.h"

UEditorNotesSettings::UEditorNotesSettings(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Labels.Add(FLabelData("Art", FLinearColor(0.f, 0.921875f, 0.166452f)));
	Labels.Add(FLabelData("Code", FLinearColor(0.911458f, 0.842272f, 0.f)));
	Labels.Add(FLabelData("Design", FLinearColor(0.916667f, 0.357338f, 0.200967f)));
	Labels.Add(FLabelData("LD", FLinearColor(0.916667f, 0.f, 0.023441f)));
	Labels.Add(FLabelData("Writing", FLinearColor(0.145833f, 0.145833f, 0.145833f)));
	Labels.Add(FLabelData("Audio", FLinearColor(0.119258f, 0.546196f, 0.838542f)));

}

TSubclassOf<AEditorNoteActor> UEditorNotesSettings::GetEditorNoteActorClass()
{
	UEditorNotesSettings* EditorNotesSettings = Cast<UEditorNotesSettings>(UEditorNotesSettings::StaticClass()->GetDefaultObject());
	return EditorNotesSettings->EditorNoteActorClass;
}

FString UEditorNotesSettings::GetNotesLevelName()
{
	UEditorNotesSettings* EditorNotesSettings = Cast<UEditorNotesSettings>(UEditorNotesSettings::StaticClass()->GetDefaultObject());
	return EditorNotesSettings->NotesLevelName;
}

bool UEditorNotesSettings::GetAutoRotate()
{
	UEditorNotesSettings* EditorNotesSettings = Cast<UEditorNotesSettings>(UEditorNotesSettings::StaticClass()->GetDefaultObject());
	return EditorNotesSettings->bAutoRotate;
}

TArray<FLabelData> UEditorNotesSettings::GetEditorNoteLabels()
{
	UEditorNotesSettings* EditorNotesSettings = Cast<UEditorNotesSettings>(UEditorNotesSettings::StaticClass()->GetDefaultObject());
	return EditorNotesSettings->Labels;
}
