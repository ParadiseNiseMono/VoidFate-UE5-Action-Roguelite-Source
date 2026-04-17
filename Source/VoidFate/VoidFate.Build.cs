// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class VoidFate : ModuleRules
{
	public VoidFate(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core",
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"EnhancedInput", 
			"GameplayTags",
			"UMG",
			"CommonInput",
			"PropertyPath",
			"PreLoadScreen",
			"GameplayTasks",
			"GameplayAbilities", 
			"AIModule",
			"MotionWarping",
			"Niagara",
			"NavigationSystem",
			"CinematicCamera"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { "AnimGraphRuntime" });

		// Uncomment if you are using Slate UI
		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
