// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Exfil : ModuleRules
{
    public Exfil(ReadOnlyTargetRules Target) : base(Target)
    {
   
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "EnhancedInput",
            "GameplayTags",
            "GameplayTasks",
            "GameplayAbilities",
            "AIModule",
            "NavigationSystem",
            "Networking",
            "UMG",
            "Niagara",
            "OnlineSubsystem",
            "OnlineSubsystemSteam",
            "Paper2D",
            "OnlineSubsystemUtils",
            "Voice"
        });

        // Uncomment if you are using Slate UI
        PrivateDependencyModuleNames.AddRange(new string[] { "UMG", "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
