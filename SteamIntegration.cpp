// SteamIntegration.cpp
// Steamworks SDK集成实现
#include "SteamIntegration.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"

// 如果你已经安装了Steamworks SDK，取消下面的注释并链接相应的库
// #include "steam_api.h"

USteamIntegration::USteamIntegration()
{
    bSteamInitialized = false;
}

void USteamIntegration::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    
    // 初始化Steam
    bSteamInitialized = InitializeSteam();
    
    if (bSteamInitialized)
    {
        UE_LOG(LogTemp, Log, TEXT("Steam initialized successfully!"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to initialize Steam. Running without Steam integration."));
    }
}

void USteamIntegration::Deinitialize()
{
    if (bSteamInitialized)
    {
        ShutdownSteam();
    }
    
    Super::Deinitialize();
}

bool USteamIntegration::InitializeSteam()
{
    // 注意：实际项目中需要链接Steamworks SDK
    // 这里提供框架代码，实际使用时需要：
    // 1. 下载Steamworks SDK
    // 2. 将steam_api.lib添加到项目依赖
    // 3. 将steam_api.dll放到可执行文件目录
    // 4. 取消下面代码的注释
    
    /*
    if (SteamAPI_Init())
    {
        // 获取Steam用户ID
        CSteamID UserId = SteamUser()->GetSteamID();
        UE_LOG(LogTemp, Log, TEXT("Steam User ID: %llu"), UserId.ConvertToUint64());
        
        // 请求成就数据
        SteamUserStats()->RequestCurrentStats();
        
        return true;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("SteamAPI_Init failed!"));
        return false;
    }
    */
    
    // 模拟初始化成功（开发阶段）
    UE_LOG(LogTemp, Warning, TEXT("Steam integration is in simulation mode. Deploy with real Steamworks SDK for production."));
    return false; // 设为false以使用模拟模式
}

void USteamIntegration::ShutdownSteam()
{
    // if (bSteamInitialized)
    // {
    //     SteamAPI_Shutdown();
    // }
    
    UE_LOG(LogTemp, Log, TEXT("Steam shutdown complete."));
}

FString USteamIntegration::GetSteamUsername() const
{
    if (!bSteamInitialized) return "NotInitialized";
    
    // return FString(SteamFriends()->GetPersonaName());
    return "SimulatedUser"; // 模拟模式
}

FString USteamIntegration::GetSteamID() const
{
    if (!bSteamInitialized) return "NotInitialized";
    
    // CSteamID UserId = SteamUser()->GetSteamID();
    // return FString::Printf(TEXT("%llu"), UserId.ConvertToUint64());
    return "123456789"; // 模拟模式
}

void USteamIntegration::UnlockAchievement(const FString& AchievementId)
{
    if (!bSteamInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("UnlockAchievement called but Steam not initialized. Achievement: %s"), *AchievementId);
        return;
    }

    // 检查是否已解锁
    if (IsAchievementUnlocked(AchievementId))
    {
        return; // 已解锁，无需重复
    }

    // 实际Steam API调用
    /*
    if (SteamUserStats()->SetAchievement(TCHAR_TO_UTF8(*AchievementId)))
    {
        SteamUserStats()->StoreStats(); // 保存到Steam服务器
        
        UE_LOG(LogTemp, Log, TEXT("Achievement unlocked: %s"), *AchievementId);
        
        // 触发事件
        OnAchievementUnlocked.Broadcast(AchievementId);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to unlock achievement: %s"), *AchievementId);
    }
    */
    
    // 模拟模式
    AchievementCache.Add(AchievementId, true);
    UE_LOG(LogTemp, Log, TEXT("[SIMULATION] Achievement unlocked: %s"), *AchievementId);
    OnAchievementUnlocked.Broadcast(AchievementId);
}

bool USteamIntegration::IsAchievementUnlocked(const FString& AchievementId)
{
    if (!bSteamInitialized) return false;

    // 实际Steam API调用
    /*
    bool bUnlocked = false;
    if (SteamUserStats()->GetAchievement(TCHAR_TO_UTF8(*AchievementId), &bUnlocked))
    {
        return bUnlocked;
    }
    return false;
    */
    
    // 模拟模式
    return AchievementCache.Contains(AchievementId) ? AchievementCache[AchievementId] : false;
}

TArray<FSteamAchievement> USteamIntegration::GetAllAchievements() const
{
    TArray<FSteamAchievement> Achievements;
    
    // 实际项目中应从Steam后台获取成就列表
    // 这里提供示例数据
    
    Achievements.Add({"FLY_HIGH", "高空飞行", "在1000米以上高度飞行", false});
    Achievements.Add({"SPEED_DEMON", "速度恶魔", "达到最大速度", false});
    Achievements.Add({"FIRST_FLIGHT", "首次飞行", "完成第一次飞行", false});
    Achievements.Add({"LONG_FLIGHT", "长途飞行", "累计飞行100小时", false});
    
    return Achievements;
}

void USteamIntegration::ResetAllAchievements()
{
    if (!bSteamInitialized) return;

    // if (SteamUserStats()->ResetAllAchievements())
    // {
    //     SteamUserStats()->StoreStats();
    //     AchievementCache.Empty();
    //     UE_LOG(LogTemp, Log, TEXT("All achievements reset."));
    // }
    
    // 模拟模式
    AchievementCache.Empty();
    UE_LOG(LogTemp, Warning, TEXT("[SIMULATION] All achievements reset."));
}

void USteamIntegration::UploadLeaderboardScore(const FString& LeaderboardName, int32 Score)
{
    if (!bSteamInitialized) return;

    // SteamLeaderboard_t hLeaderboard = SteamUserStats()->FindLeaderboard(TCHAR_TO_UTF8(*LeaderboardName));
    // if (hLeaderboard != NULL)
    // {
    //     SteamUserStats()->UploadLeaderboardScore(hLeaderboard, ELeaderboardUploadScoreMethodKeepBest, Score, NULL, 0);
    // }
    
    UE_LOG(LogTemp, Log, TEXT("[SIMULATION] Uploaded score %d to leaderboard %s"), Score, *LeaderboardName);
}

void USteamIntegration::DownloadLeaderboard(const FString& LeaderboardName, int32 RangeStart, int32 RangeEnd)
{
    if (!bSteamInitialized) return;

    // 实际项目中需要实现回调
    UE_LOG(LogTemp, Log, TEXT("[SIMULATION] Downloading leaderboard %s (Range: %d-%d)"), *LeaderboardName, RangeStart, RangeEnd);
}

void USteamIntegration::CreateSession(int32 MaxPlayers)
{
    if (!bSteamInitialized) return;

    // 使用UE5的Online Subsystem创建会话
    IOnlineSubsystem* OSS = IOnlineSubsystem::Get("Steam");
    if (OSS)
    {
        IOnlineSessionPtr SessionInterface = OSS->GetSessionInterface();
        // 创建会话逻辑...
    }
}

void USteamIntegration::JoinSession(const FString& SessionId)
{
    if (!bSteamInitialized) return;

    // 加入会话逻辑...
}

void USteamIntegration::FindSessions(bool bIsLanMatch)
{
    if (!bSteamInitialized) return;

    // 查找会话逻辑...
}

bool USteamIntegration::SaveToSteamCloud(const FString& FileName, const TArray<uint8>& Data)
{
    if (!bSteamInitialized) return false;

    // if (SteamRemoteStorage()->FileWrite(TCHAR_TO_UTF8(*FileName), Data.GetData(), Data.Num()))
    // {
    //     return true;
    // }
    
    UE_LOG(LogTemp, Log, TEXT("[SIMULATION] Saved %d bytes to Steam Cloud file: %s"), Data.Num(), *FileName);
    return true; // 模拟成功
}

bool USteamIntegration::LoadFromSteamCloud(const FString& FileName, TArray<uint8>& OutData)
{
    if (!bSteamInitialized) return false;

    // 模拟加载
    UE_LOG(LogTemp, Log, TEXT("[SIMULATION] Loaded from Steam Cloud file: %s"), *FileName);
    return true;
}
