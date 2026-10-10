// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SnakeEatApples : ModuleRules
{
	public SnakeEatApples(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"NavigationSystem",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"Niagara",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"SnakeEatApples",
			"SnakeEatApples/Variant_Strategy",
			"SnakeEatApples/Variant_Strategy/UI",
			"SnakeEatApples/Variant_TwinStick",
			"SnakeEatApples/Variant_TwinStick/AI",
			"SnakeEatApples/Variant_TwinStick/Gameplay",
			"SnakeEatApples/Variant_TwinStick/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
