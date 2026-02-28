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
- **平台**：Windows (Win32)
- **构建**：Visual Studio (v143 工具集)

## 项目结构

```
PlayCards/
├── Classes/
│   ├── AppDelegate.cpp/h          # 应用入口，启动关卡选择场景
│   ├── HelloWorldScene.cpp/h      # 示例场景（未使用）
│   ├── configs/models/           # 数据模型
│   │   ├── CardResConfig.h
│   │   ├── GameModel.h
│   │   └── LevelConfig.h
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
├── res/res/
│   ├── playfield.json            # 默认关卡配置
│   └── level_1.json, level_2.json, level_3.json  # 各关卡配置（可选）
├── proj.win32/                   # Visual Studio 工程
└── cocos2d/                      # Cocos2d-x 引擎
```

## 关卡配置

关卡配置为 JSON 格式，路径：`res/res/level_{id}.json`，若不存在则回退到 `playfield.json`。

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

### 环境要求

- Windows
- Visual Studio 2019 或更高版本（含 C++ 桌面开发工作负载）
- Cocos2d-x 已包含在项目 `cocos2d/` 目录中

### 构建步骤

1. 使用 Visual Studio 打开 `proj.win32\PlayCards.sln`（或直接打开 `proj.win32\PlayCards.vcxproj`）
2. 选择 **Debug** 或 **Release** 配置
3. 生成解决方案（Ctrl+Shift+B）

### 运行

1. 将 `res` 目录设置为工作目录，或确保可执行文件运行时能访问 `res/res/` 下的资源
2. 运行生成的 `PlayCards.exe`（通常在 `Debug.win32/` 或 `Release.win32/` 目录）

## 设计分辨率

- 设计分辨率：1080 × 2080
- 主牌区：1080 × 1500
- 堆牌区：1080 × 580

## 许可证

本项目使用 Cocos2d-x 引擎，遵循其相应许可。应用代码部分可自行约定许可。
