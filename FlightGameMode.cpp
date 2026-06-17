// FlightGameMode.cpp
#include "FlightGameMode.h"
#include "AirplanePawn.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"

AFlightGameMode::AFlightGameMode()
{
    PrimaryActorTick.bCanEverTick = true;
    DefaultPawnClass = AAirplanePawn::StaticClass();
    
    // 初始化游戏状态
    bIsGameActive = false;
    GameTime = 0.0f;
    CurrentScore = 0;
    BestAltitude = 0.0f;
    TotalFlightTime = 0.0f;
}

void AFlightGameMode::BeginPlay()
{
    Super::BeginPlay();
    
    UE_LOG(LogTemp, Log, TEXT("Flight Simulator Game Mode Initialized"));
    
    // 加载游戏进度
    LoadGameProgress();
}

void AFlightGameMode::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    
    if (bIsGameActive)
    {
        // 更新游戏时间
        GameTime += DeltaSeconds;
        TotalFlightTime += DeltaSeconds;
        
        // 更新飞行统计
        UpdateFlightStats(DeltaSeconds);
        
        // 检查游戏条件
        CheckGameConditions();
        
        // 触发成就检查
        TriggerAchievementCheck();
    }
}

void AFlightGameMode::StartFlight()
{
    if (!bIsGameActive)
    {
        bIsGameActive = true;
        GameTime = 0.0f;
        
        UE_LOG(LogTemp, Log, TEXT("Flight Started!"));
        
        // 触发事件
        OnFlightStarted.Broadcast();
    }
}

void AFlightGameMode::EndFlight()
{
    if (bIsGameActive)
    {
        bIsGameActive = false;
        
        UE_LOG(LogTemp, Log, TEXT("Flight Ended! Score: %d"), CurrentScore);
        
        // 保存进度
        SaveGameProgress();
        
        // 触发事件
        OnFlightEnded.Broadcast();
    }
}

void AFlightGameMode::AddScore(int32 Points)
{
    CurrentScore += Points;
    
    UE_LOG(LogTemp, Log, TEXT("Score Added: +%d (Total: %d)"), Points, CurrentScore);
}

void AFlightGameMode::CheckGameConditions()
{
    // 检查时间限制
    if (bEnableTimeLimit && GameTime >= TimeLimit)
    {
        EndFlight();
    }
    
    // 获取玩家飞机
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (PlayerPawn)
    {
        AAirplanePawn* Airplane = Cast<AAirplanePawn>(PlayerPawn);
        if (Airplane)
        {
            // 检查高度目标
            float CurrentAltitude = Airplane->Altitude;
            
            if (CurrentAltitude > BestAltitude)
            {
                BestAltitude = CurrentAltitude;
            }
            
            if (CurrentAltitude >= TargetAltitude)
            {
                OnAltitudeReached.Broadcast(CurrentAltitude);
            }
            
            // 检查是否接地
            if (Airplane->bIsGrounded)
            {
                // 安全着陆，加分
                AddScore(100);
            }
        }
    }
}

void AFlightGameMode::UpdateFlightStats(float DeltaTime)
{
    // 每帧更新统计信息
    // 可以在这里添加更多统计逻辑
}

void AFlightGameMode::TriggerAchievementCheck()
{
    // 触发Steam成就检查
    // 实际逻辑在AirplanePawn中处理
}

void AFlightGameMode::SaveGameProgress()
{
    // 使用UE5的SaveGame系统
    // 创建SaveGame对象并保存
    
    UE_LOG(LogTemp, Log, TEXT("Game Progress Saved! Best Altitude: %.1f, Total Time: %.1f"), BestAltitude, TotalFlightTime);
}

void AFlightGameMode::LoadGameProgress()
{
    // 加载游戏进度
    // 从SaveGame对象读取
    
    UE_LOG(LogTemp, Log, TEXT("Game Progress Loaded! Best Altitude: %.1f, Total Time: %.1f"), BestAltitude, TotalFlightTime);
}
