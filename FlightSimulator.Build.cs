// FlightSimulator.Build.cs
// UE5模块构建配置 - 定义依赖项和编译设置

using UnrealBuildTool;

public class FlightSimulator : ModuleRules
{
    public FlightSimulator(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        // ========== 公共依赖模块 ==========
        PublicDependencyModuleNames.AddRange(new string[] 
        {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "PhysicsCore",
            "Chaos",
            "ChaosVehicles",
            "OnlineSubsystem",
            "OnlineSubsystemUtils",
            "OnlineSubsystemSteam",
            "Sockets",
            "Networking",
            "Slate",
            "SlateCore",
            "UMG",
            "GameplayTags",
            "GameplayTasks",
            "NavigationSystem",
            "AIModule",
            "Niagara",
            "Particles",
            "RenderCore",
            "RHI",
            "ShaderCore"
        });

        // ========== 私有依赖模块 ==========
        PrivateDependencyModuleNames.AddRange(new string[]
        {
            "RenderCore",
            "RHI",
            "ShaderCore",
            "AssetRegistry",
            "ApplicationCore",
            "Projects",
            "Json",
            "JsonUtilities",
            "HTTP",
            "WebSockets",
            "Networking",
            "Sockets",
            "OnlineSubsystem",
            "OnlineSubsystemUtils"
        });

        // ========== Steamworks SDK 配置 ==========
        // 如果需要集成Steamworks SDK，取消下面的注释并配置路径
        /*
        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            // Steamworks SDK 路径
            string SteamSDKPath = Path.Combine(ModuleDirectory, "../../ThirdParty/Steamworks");
            
            // 添加包含路径
            PublicIncludePaths.Add(Path.Combine(SteamSDKPath, "public"));
            
            // 添加库文件
            PublicAdditionalLibraries.Add(Path.Combine(SteamSDKPath, "redistributable_bin/win64/steam_api64.lib"));
            
            // 定义宏
            PublicDefinitions.Add("WITH_STEAM=1");
        }
        else
        {
            PublicDefinitions.Add("WITH_STEAM=0");
        }
        */

        // ========== 编译设置 ==========
        // 启用异常处理
        bEnableExceptions = true;

        // 启用RTTI
        bUseRTTI = true;

        // 优化设置
        if (Target.Configuration == UnrealTargetConfiguration.Shipping)
        {
            // 发布版本优化
            OptimizeCode = CodeOptimization.Always;
        }
        else
        {
            // 开发版本不优化（便于调试）
            OptimizeCode = CodeOptimization.Never;
        }

        // ========== 预处理器定义 ==========
        PublicDefinitions.Add("FLIGHTSIMULATOR_MODULE=1");
        
        // Steam模拟模式（开发阶段使用）
        PublicDefinitions.Add("STEAM_SIMULATE=1");

        // ========== 包含路径 ==========
        PublicIncludePaths.AddRange(
            new string[] 
            {
                Path.Combine(ModuleDirectory, "Public"),
                Path.Combine(ModuleDirectory, "Classes")
            }
        );

        PrivateIncludePaths.AddRange(
            new string[] 
            {
                Path.Combine(ModuleDirectory, "Private"),
                Path.Combine(ModuleDirectory, "Classes")
            }
        );
    }
}
