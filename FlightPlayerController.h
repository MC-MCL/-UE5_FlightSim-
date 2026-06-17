// FlightPlayerController.h
// 玩家控制器 - 处理输入和UI交互
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FlightPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPauseToggled);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMenuOpened);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMenuClosed);

UCLASS()
class FLIGHTSIMULATOR_API AFlightPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    AFlightPlayerController();

protected:
    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;

    // ========== UI状态 ==========
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
    bool bIsPauseMenuOpen = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
    bool bIsMapOpen = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
    bool bIsHUDVisible = true;

    // ========== 输入绑定 ==========
    void PauseGame();
    void ToggleMap();
    void ToggleHUD();
    void TakeScreenshot();
    void QuickSave();
    void QuickLoad();

    // ========== Steam功能 ==========
    UFUNCTION(BlueprintCallable, Category = "Steam")
    void ShowSteamOverlay();

    UFUNCTION(BlueprintCallable, Category = "Steam")
    void InviteFriendToGame();

    UFUNCTION(BlueprintCallable, Category = "Steam")
    void ShowLeaderboard();

    // ========== 游戏功能 ==========
    UFUNCTION(BlueprintCallable, Category = "Game")
    void RestartFlight();

    UFUNCTION(BlueprintCallable, Category = "Game")
    void QuitToMenu();

    UFUNCTION(BlueprintCallable, Category = "Game")
    void QuitGame();

    // ========== 事件委托 ==========
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnPauseToggled OnPauseToggled;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnMenuOpened OnMenuOpened;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnMenuClosed OnMenuClosed;

    // ========== HUD控制 ==========
    UFUNCTION(BlueprintCallable, Category = "HUD")
    void ShowHUD();

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void HideHUD();

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void ToggleHUDVisibility();

    // ========== 输入模式 ==========
    UFUNCTION(BlueprintCallable, Category = "Input")
    void SetUIInputMode();

    UFUNCTION(BlueprintCallable, Category = "Input")
    void SetGameInputMode();

private:
    class AAirplanePawn* ControlledAirplane;
};
