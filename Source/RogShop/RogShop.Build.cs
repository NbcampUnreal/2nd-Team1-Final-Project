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
            Path.Combine(ModuleDirectory, "Character", "Monster"),
            Path.Combine(ModuleDirectory, "Controller"),
            Path.Combine(ModuleDirectory, "Widget", "DunShop"),
            Path.Combine(ModuleDirectory, "Widget", "Mainmenu"),
            Path.Combine(ModuleDirectory, "Widget", "Dungeon"),
            Path.Combine(ModuleDirectory, "Widget", "InGame"),
            Path.Combine(ModuleDirectory, "AnimInstances"),
            Path.Combine(ModuleDirectory, "AnimNotifyState"),
            Path.Combine(ModuleDirectory, "AnimNotify"),
            Path.Combine(ModuleDirectory, "ActorComponent"),
            Path.Combine(ModuleDirectory, "CheatManager"),
            Path.Combine(ModuleDirectory, "GameModeBase"),
            Path.Combine(ModuleDirectory, "GameInstance"),
            Path.Combine(ModuleDirectory, "Actor", "Dungeon"),
            Path.Combine(ModuleDirectory, "Actor", "Dungeon", "Weapon"),
            Path.Combine(ModuleDirectory, "Actor", "MapGenerator"),
            Path.Combine(ModuleDirectory, "Actor", "Dungeon", "GroundItem"),
            Path.Combine(ModuleDirectory, "Actor", "Dungeon", "Relic"),
            Path.Combine(ModuleDirectory, "Actor", "Dungeon", "Altar"),
            Path.Combine(ModuleDirectory, "Actor", "Dungeon", "TreasureChest"),
            Path.Combine(ModuleDirectory, "Actor", "BaseArea"),
            Path.Combine(ModuleDirectory, "Interface"),
            Path.Combine(ModuleDirectory, "DataTable"),
            Path.Combine(ModuleDirectory, "Object", "Relic"),
            Path.Combine(ModuleDirectory, "GameInstanceSubsystem"),
            Path.Combine(ModuleDirectory, "SaveGame"),
            Path.Combine(ModuleDirectory, "SaveGame", "Dungeon"),
            Path.Combine(ModuleDirectory, "SaveGame", "Tycoon"),
            Path.Combine(ModuleDirectory, "SaveGame", "Option"),
            Path.Combine(ModuleDirectory, "Struct"),
            Path.Combine(ModuleDirectory, "Enum"),
            Path.Combine(ModuleDirectory, "DeveloperSettings"),
        });

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "AIModule", "NavigationSystem", "DeveloperSettings", "SkeletalMerging", "UMG", "PhysicsCore", "Niagara" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
