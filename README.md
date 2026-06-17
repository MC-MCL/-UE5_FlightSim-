# UE5 Flight Simulator

**一个开箱即用的 UE5 飞行模拟器 C++ 模板，Steam 成就/排行榜/云存档已集成。**

✅ 完整飞行物理（Chaos 引擎）
✅ 多视角切换 + HUD 仪表
✅ Steam 平台完整接入
✅ 按文档操作，可直接打包上架 Steam

🚀 [5 分钟快速开始](#-快速开始) | 🎮 [控制说明](#-游戏控制) | 📦 [打包 Steam](#-打包上架-steam)

## 🎮 项目特性

### 核心功能
- ✅ **真实飞行物理** - 基于UE5 Chaos物理引擎
- ✅ **多视角控制** - 驾驶舱视角、第三人称视角
- ✅ **完整HUD系统** - 速度表、高度表、罗盘、油门指示
- ✅ **Steam集成** - 成就、排行榜、云存档、多人联机
- ✅ **可上架Steam** - 完整配置，符合Steamworks规范

### 技术栈
- **引擎**: Unreal Engine 5.3 / 5.4
- **编程语言**: C++
- **物理引擎**: Chaos Physics
- **网络**: Online Subsystem Steam
- **渲染**: Nanite + Lumen
- **音频**: MetaSounds

---

## 📦 项目结构

```
UE5_FlightSim/
├── Source/FlightSimulator/       # C++源代码
│   ├── AirplanePawn.h/cpp       # 飞行器控制
│   ├── FlightGameMode.h/cpp     # 游戏模式
│   ├── FlightPlayerController.h/cpp  # 玩家控制器
│   ├── FlightHUD.h/cpp         # HUD界面
│   ├── SteamIntegration.h/cpp   # Steam集成
│   └── FlightSimulator.Build.cs # 构建配置
├── Config/                       # 配置文件
│   ├── DefaultEngine.ini         # 引擎配置
│   └── DefaultInput.ini         # 输入映射
├── Content/                      # 资源文件（UE编辑器创建）
│   ├── Maps/                    # 地图
│   ├── Blueprints/              # 蓝图
│   ├── Meshes/                  # 3D模型
│   └── Materials/               # 材质
└── README.md                    # 本文件
```

---

## 🚀 快速开始

### 1. 安装虚幻引擎5

1. 下载 [Epic Games Launcher](https://www.epicgames.com/store/zh-CN/download)
2. 安装 **UE 5.3** 或 **UE 5.4**
3. 至少需要 **100GB** 磁盘空间

### 2. 创建项目

1. 打开UE5编辑器
2. 选择 **"游戏"** → **"空白模板"**
3. 项目设置:
   - **蓝图或C++**: 选择 **C++**
   - **目标平台**: 桌面
   - **质量预设**: 电影级
   - **启动内容**: 包含
4. 项目名称: `FlightSimulator`
5. 点击 **"创建"**

### 3. 添加代码文件

将以下文件复制到你的项目 `Source/FlightSimulator/` 目录:

- `AirplanePawn.h` / `AirplanePawn.cpp`
- `FlightGameMode.h` / `FlightGameMode.cpp`
- `FlightPlayerController.h` / `FlightPlayerController.cpp`
- `FlightHUD.h` / `FlightHUD.cpp`
- `SteamIntegration.h` / `SteamIntegration.cpp`
- `FlightSimulator.Build.cs`

### 4. 配置文件

将配置文件复制到 `Config/` 目录:

- `DefaultEngine.ini`
- `DefaultInput.ini`

### 5. 编译项目

1. 在UE5编辑器中，点击 **"编译"** 按钮
2. 或者，右键项目文件夹 → **"Generate Visual Studio project files"**
3. 打开 `.sln` 文件，用Visual Studio编译

---

## 🎮 游戏控制

### 键盘控制
| 按键 | 功能 |
|------|------|
| **W / S** | 油门增减 |
| **A / D** | 横滚（左右倾斜） |
| **↑ / ↓** | 俯仰（抬头/低头） |
| **← / →** | 偏航（左转/右转） |
| **Q / E** | 横滚（左倾/右倾） |
| **空格 / Shift** | 上升/下降 |
| **F** | 襟翼开关 |
| **M** | 地图开关 |
| **H** | HUD开关 |
| **F5** | 快速保存 |
| **F9** | 快速加载 |
| **F12** | 截图（自动上传Steam） |
| **ESC** | 暂停菜单 |

### 鼠标控制
- **鼠标移动** - 控制偏航和俯仰
- **滚轮** - 调整油门

### 游戏手柄（可选）
- **左摇杆** - 横滚/偏航
- **右摇杆** - 俯仰
- **LT/RT** - 油门减/增
- **Start** - 暂停

---

## 🔧 Steam集成

### 1. 注册Steamworks开发者

1. 访问 [Steamworks](https://partner.steamgames.com/)
2. 支付 **$100 USD** 注册费
3. 填写税务信息和银行账户

### 2. 配置Steam AppID

在 `DefaultEngine.ini` 中修改:
```ini
[OnlineSubsystemSteam]
SteamDevAppId=你的AppID  # 替换为你自己的AppID
```

测试阶段可以使用 `480` (Steam提供的测试AppID)。

### 3. 集成Steamworks SDK

1. 从Steamworks后台下载 **Steamworks SDK**
2. 将SDK解压到 `ThirdParty/Steamworks/` 目录
3. 在 `FlightSimulator.Build.cs` 中取消Steam相关注释
4. 将 `steam_api.dll` / `steam_api64.dll` 放到可执行文件目录

### 4. Steam功能

#### 成就系统
在 `SteamIntegration.cpp` 中配置成就:
```cpp
TArray<FSteamAchievement> Achievements;
Achievements.Add({"FLY_HIGH", "高空飞行", "在1000米以上飞行", false});
Achievements.Add({"SPEED_DEMON", "速度恶魔", "达到最大速度", false});
```

#### 排行榜
```cpp
// 上传分数
SteamIntegration->UploadLeaderboardScore("Fastest_Lap", Score);

// 下载排行榜
SteamIntegration->DownloadLeaderboard("Fastest_Lap", 1, 10);
```

#### 云存档
```cpp
// 保存到Steam云
TArray<uint8> SaveData;
SteamIntegration->SaveToSteamCloud("savegame.sav", SaveData);

// 从Steam云加载
TArray<uint8> LoadData;
SteamIntegration->LoadFromSteamCloud("savegame.sav", LoadData);
```

---

## 📦 打包上架Steam

### 1. 准备发行素材

| 素材类型 | 尺寸 | 格式 |
|---------|------|------|
| **游戏图标** | 512x512 | PNG (透明背景) |
| **封面图** | 616x353 | JPEG |
| **背景图** | 1920x620 | JPEG |
| **截图** | 1920x1080+ | JPEG (至少5张) |
| **宣传视频** | 1920x1080 | MP4 (30-90秒) |
| **横幅图** | 460x215 | JPEG |

### 2. 配置项目设置

1. 打开 **"编辑"** → **"项目设置"**
2. 设置 **"项目名称"**、**"版本号"**
3. 配置 **"地图和模式"** (默认地图)
4. 设置 **"打包"** → **"包含调试文件"** = false

### 3. 打包项目

1. 点击 **"文件"** → **"打包项目"** → **"Windows"**
2. 选择输出目录
3. 等待打包完成（生成 `.exe` 文件）

### 4. 上传到Steamworks

1. 登录 [Steamworks后台](https://partner.steamgames.com/)
2. 点击 **"注册新应用程序"**
3. 填写游戏信息
4. 上传所有素材
5. 上传游戏包（`.exe` + 依赖文件）
6. 设置价格和区域
7. 提交审核

### 5. 审核与发布

- **审核时间**: 1-5个工作日
- **首发建议**: 先设置 **"即将推出"** 页面，收集愿望单
- **促销活动**: 参加Steam新品节、季节特卖

---

## 💰 收入与分成

- **Steam分成**: 30% (前1000万美元)，之后25%
- **示例**: 游戏定价 ¥68
  - 玩家支付: ¥68
  - Steam扣除: ¥20.4
  - 你到账: ¥47.6 (税前)

---

## 🐛 常见问题

### Q: 编译失败？
- 检查Visual Studio是否安装了 **"使用C++的桌面开发"** 工作负载
- 重启UE5编辑器
- 删除 `Intermediate` 和 `Binaries` 文件夹，重新生成项目文件

### Q: Steam功能不工作？
- 确保Steam客户端已登录
- 检查 `DefaultEngine.ini` 中的 `SteamDevAppId` 是否正确
- 确认 `steam_api.dll` 已放到可执行文件目录

### Q: 游戏卡顿？
- 降低渲染质量 (**"项目设置"** → **"引擎可扩展性"**)
- 减少场景中的多边形数量
- 关闭实时光照和阴影

### Q: 如何添加更多飞机？
- 在 `Content/Meshes/` 中导入新的3D模型
- 创建新的 `AirplanePawn` 蓝图
- 更换 `AirplaneMesh` 组件

---

## 📚 学习资源

- [UE5官方文档](https://docs.unrealengine.com/5.3/zh-CN/)
- [Steamworks文档](https://partner.steamgames.com/doc/home)
- [UE5 C++编程指南](https://docs.unrealengine.com/5.3/zh-CN/API/)
- [B站UE5教程](https://search.bilibili.com/all?keyword=虚幻引擎5)

---

## 📄 许可证

本项目代码开源，遵循 **Apache-2.0**。

---

## 🙏 致谢

- Epic Games - 虚幻引擎5
- Valve - Steamworks SDK
- 开发人员：Duck、MLC（C++、C#）
- 建模：白帝
- 运营：血色能源
-版权声明：Copyright © Magician's Wand 2026.
Licensed under the Apache License, Version 2.0 ...
---


