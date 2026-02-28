#pragma once

#include "configs/models/LevelConfig.h"
#include "configs/models/GameModel.h"
#include "LevelConfigLoader.h"
#include "GameModelFromLevelGenerator.h"
#include "PlayFieldController.h"
#include "StackController.h"
#include "manager/UndoManager.h"
#include "cocos2d.h"
#include <memory>

//struct Node; // cocos2d 前向声明用
//namespace cocos2d { class Scene; }

namespace playcard {

/** 游戏总控：从关卡选择到创建视图、初始化子控制器、播放入场动画 */
class GameController {
public:
    GameController();
    /** 用户选择关卡后调用：加载配置 -> 生成模型 -> 初始化子控制器 -> 创建并显示 GameView -> 初始化子视图 -> 设置布局 -> 播放入场动画 */
    void startGame(int levelId);

    /** 可选：指定父节点添加游戏视图（不替换场景时使用）。默认 nullptr 表示替换当前运行场景。 */
    void setParentNode(cocos2d::Node* parent) { _parentNode = parent; }
    cocos2d::Node* getParentNode() const { return _parentNode; }

private:
    LevelConfigLoader _levelConfigLoader;
    GameModelFromLevelGenerator _gameModelGenerator;
    std::unique_ptr<GameModel> _gameModel;
    PlayFieldController _playFieldController;
    StackController _stackController;
    UndoManager _undoManager;
    cocos2d::Node* _parentNode = nullptr;
};

} // namespace playcard
