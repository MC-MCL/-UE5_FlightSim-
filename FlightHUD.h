// FlightHUD.h
// HUD界面 - 显示飞行数据、小地图、成就通知
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "FlightHUD.generated.h"

// 飞行数据结构体
USTRUCT(BlueprintType)
struct FFlightData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
    float Speed = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
    float Altitude = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
    float Throttle = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
    FRotator Rotation = FRotator::ZeroRotator;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
    float Fuel = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
    FString Location = TEXT("Unknown");
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAchievementNotify, const FString&, AchievementName);

UCLASS()
class FLIGHTSIMULATOR_API AFlightHUD : public AHUD
{
    GENERATED_BODY()

public:
    AFlightHUD();

protected:
    virtual void BeginPlay() override;
    virtual void DrawHUD() override;

    // ========== HUD元素 ==========
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
    bool bShowSpeedometer = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
    bool bShowAltimeter = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
    bool bShowCompass = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
    bool bShowMiniMap = false;

    // ========== 绘制函数 ==========
    void DrawSpeedometer();
    void DrawAltimeter();
    void DrawCompass();
    void DrawMiniMap();
    void DrawFuelGauge();
    void DrawThrottleBar();
    void DrawAchievementNotify();

    // ========== 数据获取 ==========
    FFlightData GetFlightData() const;

    // ========== 通知系统 ==========
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD|Notify")
    TArray<FString> ActiveNotifications;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD|Notify")
    float NotificationTimer = 0.0f;

public:
    // ========== 蓝图接口 ==========
    UFUNCTION(BlueprintCallable, Category = "HUD")
    void ShowNotification(const FString& Message, float Duration = 3.0f);

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void ShowAchievementUnlocked(const FString& AchievementName);

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void ToggleHUD(bool bShow);

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void SetHUDVisibility(bool bVisible);

    // ========== 事件委托 ==========
    UPROPERTY(BlueprintAssignable, Category = "HUD|Events")
    FOnAchievementNotify OnAchievementNotify;

protected:
    // 通知队列
    struct FNotification
    {
        FString Text;
        float RemainingTime;
    };

    TArray<FNotification> NotificationQueue;
};
