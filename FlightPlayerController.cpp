// FlightPlayerController.cpp
#include "FlightPlayerController.h"
#include "AirplanePawn.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Engine/Engine.h"

AFlightPlayerController::AFlightPlayerController()
{
    bShowMouseCursor = false;
    bEnableClickEvents = false;
    bEnableMouseOverEvents = false;
}

void AFlightPlayerController::BeginPlay()
{
    Super::BeginPlay();

    // 获取控制的飞机
    ControlledAirplane = Cast<AAirplanePawn>(GetPawn());
}

void AFlightPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    // 绑定输入轴
    InputComponent->BindAxis("Pitch");
    InputComponent->BindAxis("Yaw");
    InputComponent->BindAxis("Roll");
    InputComponent->BindAxis("Ascend");

    // 绑定动作映射
    InputComponent->BindAction("Pause", IE_Pressed, this, &AFlightPlayerController::PauseGame);
    InputComponent->BindAction("ToggleMap", IE_Pressed, this, &AFlightPlayerController::ToggleMap);
    InputComponent->BindAction("ToggleHUD", IE_Pressed, this, &AFlightPlayerController::ToggleHUD);
    InputComponent->BindAction("Screenshot", IE_Pressed, this, &AFlightPlayerController::TakeScreenshot);
    InputComponent->BindAction("QuickSave", IE_Pressed, this, &AFlightPlayerController::QuickSave);
    InputComponent->BindAction("QuickLoad", IE_Pressed, this, &AFlightPlayerController::QuickLoad);
}

// ========== UI控制 ==========
void AFlightPlayerController::PauseGame()
{
    if (!bIsPauseMenuOpen)
    {
        // 打开暂停菜单
        bIsPauseMenuOpen = true;
        SetUIInputMode();
        bShowMouseCursor = true;
        SetGamePaused(true);

        OnPauseToggled.Broadcast();
        UE_LOG(LogTemp, Log, TEXT("Game Paused"));
    }
    else
    {
        // 关闭暂停菜单
        bIsPauseMenuOpen = false;
        SetGameInputMode();
        bShowMouseCursor = false;
        SetGamePaused(false);

        OnPauseToggled.Broadcast();
        UE_LOG(LogTemp, Log, TEXT("Game Resumed"));
    }
}

void AFlightPlayerController::ToggleMap()
{
    bIsMapOpen = !bIsMapOpen;

    if (bIsMapOpen)
    {
        OnMenuOpened.Broadcast();
        UE_LOG(LogTemp, Log, TEXT("Map Opened"));
    }
    else
    {
        OnMenuClosed.Broadcast();
        UE_LOG(LogTemp, Log, TEXT("Map Closed"));
    }
}

void AFlightPlayerController::ToggleHUD()
{
    bIsHUDVisible = !bIsHUDVisible;

    if (bIsHUDVisible)
    {
        ShowHUD();
    }
    else
    {
        HideHUD();
    }

    UE_LOG(LogTemp, Log, TEXT("HUD %s"), bIsHUDVisible ? TEXT("Visible") : TEXT("Hidden"));
}

void AFlightPlayerController::TakeScreenshot()
{
    // 使用Steam截图功能（F12）
    // 或者自定义截图
    FScreenshotRequest::RequestScreenshot("FlightSim_Screenshot", false);

    UE_LOG(LogTemp, Log, TEXT("Screenshot Taken!"));
}

void AFlightPlayerController::QuickSave()
{
    // 快速保存
    if (AFlightGameMode* GameMode = Cast<AFlightGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
    {
        GameMode->SaveGameProgress();
        UE_LOG(LogTemp, Log, TEXT("Quick Save Completed"));
    }
}

void AFlightPlayerController::QuickLoad()
{
    // 快速加载
    if (AFlightGameMode* GameMode = Cast<AFlightGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
    {
        GameMode->LoadGameProgress();
        UE_LOG(LogTemp, Log, TEXT("Quick Load Completed"));
    }
}

// ========== Steam功能 ==========
void AFlightPlayerController::ShowSteamOverlay()
{
    // 显示Steam好友列表/商店页面
    UE_LOG(LogTemp, Log, TEXT("Show Steam Overlay - Not implemented yet"));
}

void AFlightPlayerController::InviteFriendToGame()
{
    // 邀请Steam好友加入游戏
    UE_LOG(LogTemp, Log, TEXT("Invite Friend - Not implemented yet"));
}

void AFlightPlayerController::ShowLeaderboard()
{
    // 显示Steam排行榜
    UE_LOG(LogTemp, Log, TEXT("Show Leaderboard - Not implemented yet"));
}

// ========== 游戏功能 ==========
void AFlightPlayerController::RestartFlight()
{
    // 重启当前关卡
    RestartLevel();
    UE_LOG(LogTemp, Log, TEXT("Flight Restarted"));
}

void AFlightPlayerController::QuitToMenu()
{
    // 返回主菜单（需要加载菜单关卡）
    UE_LOG(LogTemp, Log, TEXT("Quit To Menu - Not implemented yet"));
}

void AFlightPlayerController::QuitGame()
{
    // 退出游戏
    FGenericPlatformMisc::RequestExit(false);
}

// ========== HUD控制 ==========
void AFlightPlayerController::ShowHUD()
{
    bIsHUDVisible = true;
    // 显示HUD Widget
    UE_LOG(LogTemp, Log, TEXT("HUD Shown"));
}

void AFlightPlayerController::HideHUD()
{
    bIsHUDVisible = false;
    // 隐藏HUD Widget
    UE_LOG(LogTemp, Log, TEXT("HUD Hidden"));
}

void AFlightPlayerController::ToggleHUDVisibility()
{
    ToggleHUD();
}

// ========== 输入模式 ==========
void AFlightPlayerController::SetUIInputMode()
{
    FInputModeGameAndUI InputMode;
    InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    SetInputMode(InputMode);
}

void AFlightPlayerController::SetGameInputMode()
{
    FInputModeGameOnly InputMode;
    SetInputMode(InputMode);
}
