// AirplanePawn.h
// 飞行器Pawn类 - 完整的飞行物理和控制系统
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "FlightSimulator/Public/SteamIntegration.h"
#include "AirplanePawn.generated.h"

UCLASS()
class FLIGHTSIMULATOR_API AAirplanePawn : public APawn
{
    GENERATED_BODY()

public:
    AAirplanePawn();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // ========== 组件 ==========
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    class UStaticMeshComponent* AirplaneMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    class USpringArmComponent* SpringArm;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    class UCameraComponent* Camera;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    class UBoxComponent* CollisionBox;

    // ========== 飞行参数 ==========
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight|Speed")
    float MaxSpeed = 3000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight|Speed")
    float MinSpeed = 200.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight|Speed")
    float CruiseSpeed = 1500.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight|Throttle")
    float ThrottleIncrement = 0.1f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight|Throttle")
    float ThrottleSpeed = 800.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight|Rotation")
    float PitchSpeed = 80.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight|Rotation")
    float YawSpeed = 60.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight|Rotation")
    float RollSpeed = 70.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight|Physics")
    float LiftCoefficient = 8.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight|Physics")
    float DragCoefficient = 0.02f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight|Physics")
    float GravityScale = 1.0f;

    // ========== 当前状态 ==========
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Flight|State")
    float CurrentThrottle = 0.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Flight|State")
    float CurrentSpeed = 0.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Flight|State")
    float Altitude = 0.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Flight|State")
    FRotator CurrentRotation;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Flight|State")
    bool bIsGrounded = false;

private:
    // 输入值
    float InputPitch = 0.0f;
    float InputYaw = 0.0f;
    float InputRoll = 0.0f;
    float InputAscend = 0.0f;

    // 输入绑定函数
    void PitchInput(float Value);
    void YawInput(float Value);
    void RollInput(float Value);
    void ThrottleUp();
    void ThrottleDown();
    void AscendInput(float Value);
    void FlapsToggle();

    // 物理计算
    void ApplyFlightPhysics(float DeltaTime);
    void ApplyLift(float DeltaTime);
    void ApplyDrag(float DeltaTime);
    void ApplyGravity(float DeltaTime);
    void ApplyThrust(float DeltaTime);

    // Steam成就检查
    void CheckAchievements();
    void UnlockSteamAchievement(const FString& AchievementId);
};
