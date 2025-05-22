// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PigGameCore : ModuleRules
{
	public PigGameCore(ReadOnlyTargetRules Target) : base(Target)
	{
		CppStandard = CppStandardVersion.Cpp20;
        //bUseRTTI = true;

		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {});
        PrivateDependencyModuleNames.AddRange(new string[] { "Core" });
    }
}
