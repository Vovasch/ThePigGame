using UnrealBuildTool;
using System.Collections.Generic;
using System.IO;
 
public class TestsTarget : TargetRules
{
    public TestsTarget(TargetInfo Target) : base(Target)
    {
            Type = TargetType.Server;
        //LinkType = TargetLinkType.Modular;
        LaunchModuleName = "Tests";
        //ExtraModuleNames.Add("Tests");
        bIsBuildingConsoleApplication = true;
        //BuildEnvironment = TargetBuildEnvironment.Shared;
        CppStandard = CppStandardVersion.Cpp20;
        DefaultBuildSettings = BuildSettingsVersion.V5;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
        //bOverrideBuildEnvironment = true;
    }
}