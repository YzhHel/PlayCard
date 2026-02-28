#ifndef GameViewScene_H
#define GameViewScene_H

#include "cocos2d.h"
#include "CardViewSceneItem.h"
#include "configs/models/GameModel.h"

namespace playcard {
class PlayFieldController;
class StackController;
}

class GameViewScene : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();
    /** 由 GameController 调用：传入本关卡的 GameModel，用于根据数据生成 UI */
    static GameViewScene* createWithGameModel(const playcard::GameModel* model);
    virtual bool init() override;
    /** 初始化 UI：设置初始布局（由 init 内根据 _gameModel 完成） */
    void setupInitialLayout();
    /** 播放入场动画（可由 GameController 在 initView 之后调用） */
    void playEntranceAnimation();

    void bindPlayFieldController(playcard::PlayFieldController* ctrl);
    void bindStackController(playcard::StackController* ctrl);

    cocos2d::LayerColor* getMainLayer() const { return _mainLayer; }
    cocos2d::LayerColor* getPileLayer() const { return _pileLayer; }

protected:
    bool initWithGameModel(const playcard::GameModel* model);

private:
    const playcard::GameModel* _gameModel = nullptr;
    cocos2d::LayerColor* _mainLayer = nullptr;
    cocos2d::LayerColor* _pileLayer = nullptr;
    playcard::PlayFieldController* _playFieldController = nullptr;
    playcard::StackController* _stackController = nullptr;

    CREATE_FUNC(GameViewScene);
};

#endif // GameViewScene_H
