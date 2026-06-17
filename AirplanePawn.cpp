// AirplanePawn.cpp
#include "AirplanePawn.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/BoxComponent.h"
#include "Components/InputComponent.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

AAirplanePawn::AAirplanePawn()
{
    PrimaryActorTick.bCanEverTick = true;

    // 创建碰撞盒
    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
    CollisionBox->SetupAttachment(RootComponent);
    CollisionBox->SetBoxExtent(FVector(200.0f, 100.0f, 50.0f));
    CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    CollisionBox->SetCollisionProfileName(TEXT("Pawn"));

    // 创建网格体组件
    AirplaneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AirplaneMesh"));
    AirplaneMesh->SetupAttachment(CollisionBox);
    AirplaneMesh->SetSimulatePhysics(true);
    AirplaneMesh->SetMassOverrideInKg(NAME_None, 1500.0f);
    AirplaneMesh->SetLinearDamping(0.5f);
    AirplaneMesh->SetAngularDamping(0.5f);
    AirplaneMesh->SetCollisionProfileName(TEXT("PhysicsActor"));

    // 创建弹簧臂
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(AirplaneMesh);
    SpringArm->TargetArmLength = 600.0f;
    SpringArm->bUsePawnControlRotation = false;
    SpringArm->bInheritPitch = false;
    SpringArm->bInheritYaw = false;
    SpringArm->bInheritRoll = false;
    SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));

    // 创建摄像机
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
    Camera->bUsePawnControlRotation = false;

    // 设置根组件
    RootComponent = CollisionBox;

    // 初始化状态
    CurrentThrottle = 0.0f;
    CurrentSpeed = 0.0f;
    Altitude = 0.0f;
    bIsGrounded = false;
}

void AAirplanePawn::BeginPlay()
{
    Super::BeginPlay();
    
    // 获取初始高度
    FHitResult Hit;
    FVector Start = GetActorLocation();
    FVector End = Start - FVector(0, 0, 100000.0f);
    
    if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility))
    {
        Altitude = (Start - Hit.Location).Size();
    }
}

void AAirplanePawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // 应用飞行物理
    ApplyFlightPhysics(DeltaTime);

    // 更新状态
    CurrentSpeed = AirplaneMesh->GetPhysicsLinearVelocity().Size();
    Altitude = GetActorLocation().Z;
    CurrentRotation = GetActorRotation();

    // 检查是否接地
    FHitResult Hit;
    FVector Start = GetActorLocation();
    FVector End = Start - FVector(0, 0, 500.0f);
    
    bIsGrounded = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility);

    // 检查Steam成就
    CheckAchievements();
}

void AAirplanePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // 绑定轴映射
    PlayerInputComponent->BindAxis("Pitch", this, &AAirplanePawn::PitchInput);
    PlayerInputComponent->BindAxis("Yaw", this, &AAirplanePawn::YawInput);
    PlayerInputComponent->BindAxis("Roll", this, &AAirplanePawn::RollInput);
    PlayerInputComponent->BindAxis("Ascend", this, &AAirplanePawn::AscendInput);

    // 绑定动作映射
    PlayerInputComponent->BindAction("ThrottleUp", IE_Pressed, this, &AAirplanePawn::ThrottleUp);
    PlayerInputComponent->BindAction("ThrottleDown", IE_Pressed, this, &AAirplanePawn::ThrottleDown);
    PlayerInputComponent->BindAction("Flaps", IE_Pressed, this, &AAirplanePawn::FlapsToggle);
}

// ========== 输入处理函数 ==========
void AAirplanePawn::PitchInput(float Value)
{
    InputPitch = Value;
}

void AAirplanePawn::YawInput(float Value)
{
    InputYaw = Value;
}

void AAirplanePawn::RollInput(float Value)
{
    InputRoll = Value;
}

void AAirplanePawn::AscendInput(float Value)
{
    InputAscend = Value;
}

void AAirplanePawn::ThrottleUp()
{
    CurrentThrottle = FMath::Clamp(CurrentThrottle + ThrottleIncrement, 0.0f, 1.0f);
}

void AAirplanePawn::ThrottleDown()
{
    CurrentThrottle = FMath::Clamp(CurrentThrottle - ThrottleIncrement, 0.0f, 1.0f);
}

void AAirplanePawn::FlapsToggle()
{
    // 襟翼切换逻辑（可扩展）
    GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Flaps Toggled"));
}

// ========== 物理计算 ==========
void AAirplanePawn::ApplyFlightPhysics(float DeltaTime)
{
    ApplyThrust(DeltaTime);
    ApplyLift(DeltaTime);
    ApplyDrag(DeltaTime);
    ApplyGravity(DeltaTime);
    ApplyRotation(DeltaTime);
}

void AAirplanePawn::ApplyThrust(float DeltaTime)
{
    if (CurrentThrottle > 0.0f)
    {
        FVector Forward = GetActorForwardVector();
        FVector ThrustForce = Forward * CurrentThrottle * ThrottleSpeed * 1000.0f;
        AirplaneMesh->AddForce(ThrustForce);
    }
}

void AAirplanePawn::ApplyLift(float DeltaTime)
{
    float Speed = AirplaneMesh->GetPhysicsLinearVelocity().Size();
    
    if (Speed > 100.0f && !bIsGrounded)
    {
        float LiftMagnitude = Speed * Speed * LiftCoefficient * 0.01f;
        FVector LiftForce = FVector(0, 0, LiftMagnitude);
        AirplaneMesh->AddForce(LiftForce);
    }
}

void AAirplanePawn::ApplyDrag(float DeltaTime)
{
    FVector Velocity = AirplaneMesh->GetPhysicsLinearVelocity();
    float Speed = Velocity.Size();
    
    if (Speed > 0.0f)
    {
        FVector DragForce = -Velocity.GetSafeNormal() * Speed * Speed * DragCoefficient;
        AirplaneMesh->AddForce(DragForce);
    }
}

void AAirplanePawn::ApplyGravity(float DeltaTime)
{
    FVector GravityForce = FVector(0, 0, -980.0f * GravityScale * AirplaneMesh->GetMass());
    AirplaneMesh->AddForce(GravityForce);
}

void AAirplanePawn::ApplyRotation(float DeltaTime)
{
    if (!bIsGrounded)
    {
        FRotator RotationDelta = FRotator(
            InputPitch * PitchSpeed * DeltaTime,
            InputYaw * YawSpeed * DeltaTime,
            InputRoll * RollSpeed * DeltaTime
        );
        
        FQuat QuatRotation = FQuat(RotationDelta);
        AirplaneMesh->AddTorqueInRadians(QuatRotation.Euler() * 100000.0f * DeltaTime);
    }
    
    // 垂直升降
    if (InputAscend != 0.0f)
    {
        FVector AscendForce = FVector(0, 0, InputAscend * 50000.0f);
        AirplaneMesh->AddForce(AscendForce);
    }
}

// ========== Steam成就 ==========
void AAirplanePawn::CheckAchievements()
{
    // 示例：飞行超过1000米解锁成就
    if (Altitude > 1000.0f)
    {
        UnlockSteamAchievement("FLY_HIGH");
    }
    
    // 示例：速度超过2000解锁成就
    if (CurrentSpeed > 2000.0f)
    {
        UnlockSteamAchievement("SPEED_DEMON");
    }
}

void AAirplanePawn::UnlockSteamAchievement(const FString& AchievementId)
{
    // 调用Steam集成类
    if (USteamIntegration* SteamIntegration = GetGameInstance()->GetSubsystem<USteamIntegration>())
    {
        SteamIntegration->UnlockAchievement(AchievementId);
    }
}
