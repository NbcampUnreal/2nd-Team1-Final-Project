// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;
using UnrealBuildTool.Rules;

public class RogShop : ModuleRules
{
	public RogShop(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(new string[] {
            Path.Combine(ModuleDirectory, "Character"),
            Path.Combine(ModuleDirectory, "Character", "AI"),
            Path.Combine(ModuleDirectory, "Controller"),
            Path.Combine(ModuleDirectory, "Widget", "DunShop"),
            Path.Combine(ModuleDirectory, "AnimInstances"),
            Path.Combine(ModuleDirectory, "AnimNotifyState"),
            Path.Combine(ModuleDirectory, "ActorComponent"),
            Path.Combine(ModuleDirectory, "CheatManager"),
            Path.Combine(ModuleDirectory, "GameInstance"),
            Path.Combine(ModuleDirectory, "Actor", "Dungeon", "Weapon"),
            Path.Combine(ModuleDirectory, "AnimNotifyState"),
            Path.Combine(ModuleDirectory, "ActorComponent"),
            Path.Combine(ModuleDirectory, "Interface"),
        });

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "AIModule", "NavigationSystem" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
