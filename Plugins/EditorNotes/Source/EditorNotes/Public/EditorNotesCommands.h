// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "EditorNotesStyle.h"

class FEditorNotesCommands : public TCommands<FEditorNotesCommands>
{
public:

	FEditorNotesCommands()
		: TCommands<FEditorNotesCommands>(TEXT("EditorNotes"), NSLOCTEXT("Contexts", "EditorNotes", "EditorNotes Plugin"), NAME_None, FEditorNotesStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};