# PlayCards

基于 Cocos2d-x 的牌类益智游戏。玩家通过选择关卡进入游戏，将桌面牌与手牌区顶牌进行匹配（点数差 1），支持撤销操作。

## 功能特性

- **关卡选择**：主界面提供关卡 1、2、3 选择入口
- **主牌区**：桌面牌区域，点击牌可与手牌顶牌匹配
- **堆牌区**：左侧备用牌（横向折叠显示）、中间顶牌、右侧回退按钮
- **匹配规则**：桌面牌与手牌顶牌点数差 1 才能匹配
- **撤销**：支持回退最近一次操作

## 技术栈

- **引擎**：Cocos2d-x (C++)
- **平台**：Windows (Win32)、Android
- **构建**：Visual Studio (Win32)、Gradle + NDK (Android)

## 项目结构

```
PlayCards/
├── Classes/
│   ├── AppDelegate.cpp/h          # 应用入口，启动关卡选择场景
│   ├── HelloWorldScene.cpp/h      # 示例场景（未使用）
│   ├── configs/
│   │   ├── PlatformLayoutConfig.h # 平台布局配置（Android/Windows 竖屏差异）
│   │   └── models/               # 数据模型
│   │       ├── CardResConfig.h
│   │       ├── GameModel.h
│   │       └── LevelConfig.h
│   ├── controller/               # 控制器
│   │   ├── GameController        # 游戏总控：关卡加载、视图创建、子控制器初始化
│   │   ├── PlayFieldController   # 主牌区点击与匹配分发
│   │   ├── StackController       # 堆牌区逻辑：翻牌、顶牌匹配、撤销
│   │   ├── LevelConfigLoader     # 关卡 JSON 配置加载
│   │   └── GameModelFromLevelGenerator
│   ├── manager/
│   │   └── UndoManager           # 撤销栈管理
│   └── views/                    # 视图
│       ├── PlayCardMainViewScene # 关卡选择界面
│       ├── GameViewScene         # 游戏主界面（主牌区 + 堆牌区）
│       ├── PlayFieldView         # 主牌区视图
│       └── CardViewSceneItem     # 单张牌视图
├── Resources/                    # 资源目录（Android 打包来源）
│   └── res/res/
│       ├── playfield.json       # 默认关卡配置
│       ├── card_general.png      # 牌面资源
│       └── ...                  # 牌面、花色等图片
├── proj.win32/                   # Visual Studio 工程
├── proj.android/                 # Android Gradle 工程
└── cocos2d/                      # Cocos2d-x 引擎
```

## 关卡配置

关卡配置为 JSON 格式，路径：`Resources/res/res/level_{id}.json`，若不存在则回退到 `playfield.json`。Android 构建时会将 `Resources/` 下的文件复制到 APK 的 assets 中。

示例结构：

```json
{
  "Playfield": [
    { "CardFace": 12, "CardSuit": 0, "Position": { "x": 250, "y": 1000 } }
  ],
  "Stack": [
    { "CardFace": 2, "CardSuit": 0, "Position": { "x": 0, "y": 0 } }
  ]
}
```

- `Playfield`：主牌区牌列表
- `Stack`：堆牌区牌列表（备用牌 + 顶牌）
- `CardFace`：牌面点数
- `CardSuit`：花色
- `Position`：主牌区牌的位置坐标

## 构建与运行

### Windows

**环境要求**：Visual Studio 2019 或更高版本（含 C++ 桌面开发工作负载）

1. 打开 `proj.win32\PlayCards.sln`
2. 选择 **Debug** 或 **Release** 配置
3. 生成解决方案（Ctrl+Shift+B）
4. 运行 `PlayCards.exe`（在 `Debug.win32/` 或 `Release.win32/` 目录，需将 `res` 设为工作目录）

### Android

**环境要求**：Android Studio、Android NDK、JDK

1. 进入 `proj.android` 目录
2. 执行 `gradlew assembleDebug`（或使用 Android Studio 打开工程并 Run）
3. 生成的 APK 位于 `app/build/outputs/apk/debug/`

**说明**：Android 为强制竖屏，布局通过 `PlatformLayoutConfig.h` 针对竖屏做了适配（设计分辨率、备用牌堆位置等）。

## 设计分辨率

- 设计分辨率：1080 × 2080（竖屏）
- 主牌区：1080 × 1500
- 堆牌区：1080 × 580

## 许可证

本项目使用 Cocos2d-x 引擎，遵循其相应许可。应用代码部分可自行约定许可。

## 部分环境搭建及学习链接

Cocos2dx-3.17环境搭建：[Cocos2d-x - 成熟、轻量、开放的跨平台解决方案](https://www.cocos.com/cocos2d-x)
基础学习参考视频：https://www.bilibili.com/video/BV1JosjeKEfe/?spm_id_from=333.788.videopod.episodes&vd_source=c590a3b8520857cec1c87b81e3a21a2a
安卓打包流程参考博客：https://blog.csdn.net/qq_40283426/article/details/104859185
