#include "GameController.h"
#include "views/GameViewScene.h"
#include "cocos2d.h"

USING_NS_CC;

namespace playcard {

GameController::GameController() = default;

void GameController::startGame(int levelId)
{
    // 1. 加载关卡配置
    LevelConfig levelConfig = _levelConfigLoader.loadLevelConfig(levelId);

    // 2. 生成 GameModel
    _gameModel = std::make_unique<GameModel>(_gameModelGenerator.generateGameModel(levelId, levelConfig));

    // 3. 初始化各子控制器
    _playFieldController.init(_gameModel.get());
    _stackController.init(_gameModel.get());
    _undoManager.init(_gameModel.get());

    // 4. 创建 GameView 并添加到父节点
    GameViewScene* gameView = GameViewScene::createWithGameModel(_gameModel.get());
    if (!gameView) {
        return;
    }
    if (_parentNode) {
        _parentNode->addChild(gameView);
    } else {
        auto director = Director::getInstance();
        if (director->getRunningScene() != nullptr) {
            director->replaceScene(gameView);
        } else {
            director->runWithScene(gameView);
        }
    }

    // 5. 初始化各子控制器的视图
    _stackController.initView(gameView);
    _playFieldController.initView(gameView);

    // 6. GameView 初始化 UI（setupInitialLayout 已在 createWithGameModel -> init 中调用）
    gameView->setupInitialLayout();

    // 7. 播放入场动画
    gameView->playEntranceAnimation();
}

} // namespace playcard
