// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TP_PATHFINDING_ASTAR : ModuleRules
{
	public TP_PATHFINDING_ASTAR(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "NavigationSystem", "AIModule", "Niagara", "EnhancedInput" });
    }
}
