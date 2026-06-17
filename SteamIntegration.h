// SteamIntegration.h
// Steamworks SDK集成 - 用于成就、排行榜、多人联机
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SteamIntegration.generated.h"

// Steam成就结构体
USTRUCT(BlueprintType)
struct FSteamAchievement
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steam")
    FString AchievementId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steam")
    FString AchievementName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steam")
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steam")
    bool bUnlocked = false;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAchievementUnlocked, const FString&, AchievementId);

UCLASS(BlueprintType)
class FLIGHTSIMULATOR_API USteamIntegration : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    USteamIntegration();

    // ========== 初始化与销毁 ==========
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    // ========== Steam API 功能 ==========
    
    /** 检查Steam是否初始化成功 */
    UFUNCTION(BlueprintCallable, Category = "Steam")
    bool IsSteamInitialized() const { return bSteamInitialized; }

    /** 获取Steam用户名 */
    UFUNCTION(BlueprintCallable, Category = "Steam")
    FString GetSteamUsername() const;

    /** 获取Steam ID */
    UFUNCTION(BlueprintCallable, Category = "Steam")
    FString GetSteamID() const;

    // ========== 成就系统 ==========
    
    /** 解锁成就 */
    UFUNCTION(BlueprintCallable, Category = "Steam|Achievements")
    void UnlockAchievement(const FString& AchievementId);

    /** 检查成就是否已解锁 */
    UFUNCTION(BlueprintCallable, Category = "Steam|Achievements")
    bool IsAchievementUnlocked(const FString& AchievementId);

    /** 获取所有成就 */
    UFUNCTION(BlueprintCallable, Category = "Steam|Achievements")
    TArray<FSteamAchievement> GetAllAchievements() const;

    /** 重置所有成就（仅开发用） */
    UFUNCTION(BlueprintCallable, Category = "Steam|Achievements")
    void ResetAllAchievements();

    // ========== 排行榜 ==========
    
    /** 上传排行榜分数 */
    UFUNCTION(BlueprintCallable, Category = "Steam|Leaderboard")
    void UploadLeaderboardScore(const FString& LeaderboardName, int32 Score);

    /** 下载排行榜数据 */
    UFUNCTION(BlueprintCallable, Category = "Steam|Leaderboard")
    void DownloadLeaderboard(const FString& LeaderboardName, int32 RangeStart = 1, int32 RangeEnd = 10);

    // ========== 多人联机 ==========
    
    /** 创建多人游戏会话 */
    UFUNCTION(BlueprintCallable, Category = "Steam|Multiplayer")
    void CreateSession(int32 MaxPlayers = 4);

    /** 加入多人游戏会话 */
    UFUNCTION(BlueprintCallable, Category = "Steam|Multiplayer")
    void JoinSession(const FString& SessionId);

    /** 查找多人游戏会话 */
    UFUNCTION(BlueprintCallable, Category = "Steam|Multiplayer")
    void FindSessions(bool bIsLanMatch = false);

    // ========== 云存档 ==========
    
    /** 保存游戏到Steam云 */
    UFUNCTION(BlueprintCallable, Category = "Steam|Cloud")
    bool SaveToSteamCloud(const FString& FileName, const TArray<uint8>& Data);

    /** 从Steam云加载游戏 */
    UFUNCTION(BlueprintCallable, Category = "Steam|Cloud")
    bool LoadFromSteamCloud(const FString& FileName, TArray<uint8>& OutData);

    // ========== 事件委托 ==========
    
    UPROPERTY(BlueprintAssignable, Category = "Steam|Events")
    FOnAchievementUnlocked OnAchievementUnlocked;

protected:
    bool bSteamInitialized = false;
    
    // 成就缓存
    TMap<FString, bool> AchievementCache;

    // 初始化Steam
    bool InitializeSteam();
    
    // 关闭Steam
    void ShutdownSteam();
};
