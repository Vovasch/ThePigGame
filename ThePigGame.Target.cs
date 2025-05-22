// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class ThePigGameTarget : TargetRules
{
	public ThePigGameTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		bOverrideBuildEnvironment = true;
		ExtraModuleNames.AddRange(new string[]{ "ThePigGame", "PigGameCore" });
		CppStandard = CppStandardVersion.Cpp20;
		DefaultBuildSettings = BuildSettingsVersion.V5;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
	}
}
