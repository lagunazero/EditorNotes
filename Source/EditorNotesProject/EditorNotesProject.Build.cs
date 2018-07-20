// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class EditorNotesProject : ModuleRules
{
	public EditorNotesProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

		PrivateDependencyModuleNames.AddRange(new string[] { "UnrealEd", "UMG", "Slate", "SlateCore", "EditorNotes", });

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true

        // Comply with Unreal's Include-What-You-Use policy (https://docs.unrealengine.com/en-us/Programming/UnrealBuildSystem/IWYUReferenceGuide)
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        bEnforceIWYU = true;

        // Improves compile time quite a bit.
        bFasterWithoutUnity = true;
    }
}
