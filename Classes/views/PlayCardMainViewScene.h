#ifndef PLAYCARDMAINVIEWSCENE_H
#define PLAYCARDMAINVIEWSCENE_H

#include "cocos2d.h"
#include <functional>
#include <memory>
#include <controller/GameController.h>


/** 主界面：关卡选择。用户选择关卡（提供关卡 ID，捕获关卡选择事件）后调用 GameController::startGame(levelId) */
class PlayCardMainViewScene : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();
    virtual bool init() override;
    CREATE_FUNC(PlayCardMainViewScene);
    ~PlayCardMainViewScene();

    /** 关卡选择事件：外部可设置回调，或使用默认的 GameController::startGame(levelId) */
    using LevelSelectedCallback = std::function<void(int levelId)>;
    void setLevelSelectedCallback(LevelSelectedCallback cb) { _onLevelSelected = std::move(cb); }

private:
    void onLevelButtonClicked(int levelId);
    void buildLevelSelectionUI();

    LevelSelectedCallback _onLevelSelected;
    std::unique_ptr<playcard::GameController> _gameController;
};

#endif // PLAYCARDMAINVIEWSCENE_H
