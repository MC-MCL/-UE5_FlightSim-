// FlightGameMode.h
// 游戏模式 - 定义游戏规则和全局状态
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FlightGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFlightStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFlightEnded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAltitudeReached, float, Altitude);

UCLASS()
class FLIGHTSIMULATOR_API AFlightGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AFlightGameMode();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;

    // ========== 游戏状态 ==========
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game State")
    bool bIsGameActive = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game State")
    float GameTime = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game State")
    int32 CurrentScore = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game State")
    float BestAltitude = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game State")
    float TotalFlightTime = 0.0f;

    // ========== 游戏参数 ==========
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Rules")
    float TargetAltitude = 5000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Rules")
    float TimeLimit = 300.0f; // 5分钟

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Rules")
    bool bEnableTimeLimit = false;

    // ========== 事件委托 ==========
    UPROPERTY(BlueprintAssignable, Category = "Game Events")
    FOnFlightStarted OnFlightStarted;

    UPROPERTY(BlueprintAssignable, Category = "Game Events")
    FOnFlightEnded OnFlightEnded;

    UPROPERTY(BlueprintAssignable, Category = "Game Events")
    FOnAltitudeReached OnAltitudeReached;

    // ========== 游戏功能 ==========
public:
    UFUNCTION(BlueprintCallable, Category = "Game")
    void StartFlight();

    UFUNCTION(BlueprintCallable, Category = "Game")
    void EndFlight();

    UFUNCTION(BlueprintCallable, Category = "Game|Score")
    void AddScore(int32 Points);

    UFUNCTION(BlueprintCallable, Category = "Game|Score")
    int32 GetCurrentScore() const { return CurrentScore; }

    UFUNCTION(BlueprintCallable, Category = "Game|Time")
    float GetGameTime() const { return GameTime; }

    UFUNCTION(BlueprintCallable, Category = "Game|Time")
    float GetRemainingTime() const { return bEnableTimeLimit ? FMath::Max(0.0f, TimeLimit - GameTime) : -1.0f; }

    UFUNCTION(BlueprintCallable, Category = "Game|Stats")
    float GetBestAltitude() const { return BestAltitude; }

    UFUNCTION(BlueprintCallable, Category = "Game|Stats")
    float GetTotalFlightTime() const { return TotalFlightTime; }

    UFUNCTION(BlueprintCallable, Category = "Game|Save")
    void SaveGameProgress();

    UFUNCTION(BlueprintCallable, Category = "Game|Save")
    void LoadGameProgress();

protected:
    void CheckGameConditions();
    void UpdateFlightStats(float DeltaTime);
    void TriggerAchievementCheck();
};
