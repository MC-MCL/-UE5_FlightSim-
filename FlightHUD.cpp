// FlightHUD.cpp
#include "FlightHUD.h"
#include "AirplanePawn.h"
#include "FlightPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "CanvasItem.h"
#include "TextureResource.h"

AFlightHUD::AFlightHUD()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AFlightHUD::BeginPlay()
{
    Super::BeginPlay();
}

void AFlightHUD::DrawHUD()
{
    Super::DrawHUD();

    if (!Canvas) return;

    // 获取飞行数据
    FFlightData FlightData = GetFlightData();

    // 绘制HUD元素
    if (bShowSpeedometer)
    {
        DrawSpeedometer();
    }

    if (bShowAltimeter)
    {
        DrawAltimeter();
    }

    if (bShowCompass)
    {
        DrawCompass();
    }

    if (bShowMiniMap)
    {
        DrawMiniMap();
    }

    // 绘制燃料表
    DrawFuelGauge();

    // 绘制油门条
    DrawThrottleBar();

    // 绘制成就通知
    DrawAchievementNotify();
}

// ========== 绘制函数 ==========
void AFlightHUD::DrawSpeedometer()
{
    FString SpeedText = FString::Printf(TEXT("Speed: %.0f km/h"), GetFlightData().Speed);
    FCanvasTextItem SpeedItem(FVector2D(Canvas->SizeX - 200, 50), FText::FromString(SpeedText), GEngine->GetMediumFont(), FLinearColor::Green);
    SpeedItem.bOutlined = true;
    SpeedItem.OutlineColor = FLinearColor::Black;
    Canvas->DrawItem(SpeedItem);
}

void AFlightHUD::DrawAltimeter()
{
    FString AltText = FString::Printf(TEXT("Altitude: %.0f m"), GetFlightData().Altitude);
    FCanvasTextItem AltItem(FVector2D(Canvas->SizeX - 200, 80), FText::FromString(AltText), GEngine->GetMediumFont(), FLinearColor::Cyan);
    AltItem.bOutlined = true;
    AltItem.OutlineColor = FLinearColor::Black;
    Canvas->DrawItem(AltItem);
}

void AFlightHUD::DrawCompass()
{
    FRotator Rotation = GetFlightData().Rotation;
    float Yaw = Rotation.Yaw;

    FString Direction;
    if (Yaw >= -45 && Yaw <= 45) Direction = TEXT("N");
    else if (Yaw > 45 && Yaw <= 135) Direction = TEXT("E");
    else if (Yaw > 135 || Yaw <= -135) Direction = TEXT("S");
    else Direction = TEXT("W");

    FString CompassText = FString::Printf(TEXT("Heading: %s (%.0f°)"), *Direction, Yaw);
    FCanvasTextItem CompassItem(FVector2D(Canvas->SizeX - 200, 110), FText::FromString(CompassText), GEngine->GetMediumFont(), FLinearColor::White);
    CompassItem.bOutlined = true;
    CompassItem.OutlineColor = FLinearColor::Black;
    Canvas->DrawItem(CompassItem);
}

void AFlightHUD::DrawMiniMap()
{
    // 小地图绘制（简化版）
    float MapSize = 150.0f;
    FVector2D MapPos(Canvas->SizeX - MapSize - 20, Canvas->SizeY - MapSize - 20);

    // 绘制地图背景
    FCanvasTileItem MapBg(MapPos, GWhiteTexture, FVector2D(MapSize, MapSize), FLinearColor(0, 0, 0.5f, 0.5f));
    Canvas->DrawItem(MapBg);

    // 绘制玩家位置点
    FVector2D PlayerPos = MapPos + FVector2D(MapSize / 2, MapSize / 2);
    FCanvasBoxItem PlayerDot(PlayerPos - FVector2D(3, 3), FVector2D(6, 6));
    PlayerDot.SetColor(FLinearColor::Red);
    Canvas->DrawItem(PlayerDot);
}

void AFlightHUD::DrawFuelGauge()
{
    float Fuel = GetFlightData().Fuel;
    FString FuelText = FString::Printf(TEXT("Fuel: %.0f%%"), Fuel);
    FLinearColor FuelColor = Fuel > 50 ? FLinearColor::Green : (Fuel > 20 ? FLinearColor::Yellow : FLinearColor::Red);

    FCanvasTextItem FuelItem(FVector2D(20, Canvas->SizeY - 100), FText::FromString(FuelText), GEngine->GetMediumFont(), FuelColor);
    FuelItem.bOutlined = true;
    FuelItem.OutlineColor = FLinearColor::Black;
    Canvas->DrawItem(FuelItem);
}

void AFlightHUD::DrawThrottleBar()
{
    float Throttle = GetFlightData().Throttle;
    FString ThrottleText = FString::Printf(TEXT("Throttle: %.0f%%"), Throttle * 100);

    FCanvasTextItem ThrottleItem(FVector2D(20, Canvas->SizeY - 70), FText::FromString(ThrottleText), GEngine->GetMediumFont(), FLinearColor::Orange);
    ThrottleItem.bOutlined = true;
    ThrottleItem.OutlineColor = FLinearColor::Black;
    Canvas->DrawItem(ThrottleItem);

    // 绘制油门条
    float BarWidth = 200.0f;
    float BarHeight = 15.0f;
    FVector2D BarPos(20, Canvas->SizeY - 50);

    // 背景
    FCanvasTileItem BarBg(BarPos, GWhiteTexture, FVector2D(BarWidth, BarHeight), FLinearColor(0.2f, 0.2f, 0.2f, 1.0f));
    Canvas->DrawItem(BarBg);

    // 前景（油门值）
    FCanvasTileItem BarFg(BarPos, GWhiteTexture, FVector2D(BarWidth * Throttle, BarHeight), FLinearColor::Orange);
    Canvas->DrawItem(BarFg);
}

void AFlightHUD::DrawAchievementNotify()
{
    // 绘制成就解锁通知
    if (NotificationQueue.Num() > 0)
    {
        FNotification& Notify = NotificationQueue[0];
        Notify.RemainingTime -= GetWorld()->GetDeltaSeconds();

        if (Notify.RemainingTime > 0)
        {
            FString NotifyText = FString::Printf(TEXT("Achievement Unlocked: %s"), *Notify.Text);
            FCanvasTextItem NotifyItem(FVector2D(Canvas->SizeX / 2 - 150, 100), FText::FromString(NotifyText), GEngine->GetMediumFont(), FLinearColor::Yellow);
            NotifyItem.bOutlined = true;
            NotifyItem.OutlineColor = FLinearColor::Black;
            Canvas->DrawItem(NotifyItem);
        }
        else
        {
            NotificationQueue.RemoveAt(0);
        }
    }
}

// ========== 数据获取 ==========
FFlightData AFlightHUD::GetFlightData() const
{
    FFlightData Data;

    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (PlayerPawn)
    {
        AAirplanePawn* Airplane = Cast<AAirplanePawn>(PlayerPawn);
        if (Airplane)
        {
            Data.Speed = Airplane->CurrentSpeed;
            Data.Altitude = Airplane->Altitude;
            Data.Throttle = Airplane->CurrentThrottle;
            Data.Rotation = Airplane->CurrentRotation;
            // Fuel 可以后续实现
        }
    }

    return Data;
}

// ========== 蓝图接口 ==========
void AFlightHUD::ShowNotification(const FString& Message, float Duration)
{
    FNotification NewNotify;
    NewNotify.Text = Message;
    NewNotify.RemainingTime = Duration;
    NotificationQueue.Add(NewNotify);
}

void AFlightHUD::ShowAchievementUnlocked(const FString& AchievementName)
{
    ShowNotification(FString::Printf(TEXT("Achievement Unlocked: %s"), *AchievementName), 5.0f);

    OnAchievementNotify.Broadcast(AchievementName);
}

void AFlightHUD::ToggleHUD(bool bShow)
{
    bShowSpeedometer = bShow;
    bShowAltimeter = bShow;
    bShowCompass = bShow;
}

void AFlightHUD::SetHUDVisibility(bool bVisible)
{
    ToggleHUD(bVisible);
}
