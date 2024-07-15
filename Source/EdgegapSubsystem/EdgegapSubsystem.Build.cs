// Copyright 2024 Cajun Pro LLC. All Rights Reserved.

using UnrealBuildTool;

public class EdgegapSubsystem : ModuleRules
{
	public EdgegapSubsystem(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		PublicDependencyModuleNames.AddRange(new[] { "Core", "HTTP", "Json", "JsonUtilities" });
		PrivateDependencyModuleNames.AddRange(new[] { "CoreUObject", "Engine", "Slate", "SlateCore" });
	}
}