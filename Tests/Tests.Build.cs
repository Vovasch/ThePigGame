using System.IO; 

namespace UnrealBuildTool.Rules {
    public class Tests : ModuleRules
    {
        public Tests(ReadOnlyTargetRules Target) : base(Target)     
        {
            PublicDependencyModuleNames.AddRange(new string[] { });
            PrivateDependencyModuleNames.AddRange(
                new string[]
                {                   
                    "PigGameCore",
                    "GoogleTest",
                }
            );

            CppStandard = CppStandardVersion.Cpp20;
        

            //string googleTestBasePath = Path.Combine(ModuleDirectory, "../../packages/gtest.1.7.0/build/native/");

            //PublicSystemIncludePaths.Add(Path.Combine(googleTestBasePath, "include/")); 
            // PublicSystemIncludePaths.Add(Path.Combine(googleTestBasePath, "googlemock", "include"));        
            // PublicSystemIncludePaths.Add(Path.Combine(googleTestBasePath, "googletest"));       
            // PublicSystemIncludePaths.Add(Path.Combine(googleTestBasePath, "googletest", "include"));

            PrivatePCHHeaderFile = "Private/Tests.h";

            //PublicAdditionalLibraries.Add(Path.Combine(googleTestBasePath, "lib/Release/gtest.lib"));
        }      
    }
}