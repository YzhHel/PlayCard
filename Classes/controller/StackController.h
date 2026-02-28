#pragma once

#include "configs/models/GameModel.h"

class GameViewScene;

namespace playcard {

/** 堆牌区（Stack）逻辑控制 */
class StackController {
public:
    void init(const GameModel* gameModel);
    /** 在 GameView 创建后调用，绑定堆牌区视图 */
    void initView(GameViewScene* gameView);
private:
    const GameModel* _gameModel = nullptr;
    GameViewScene* _gameView = nullptr;
};

} // namespace playcard
