#pragma once

#include "configs/models/GameModel.h"

class GameViewScene;

namespace playcard {

/** 主牌区（Playfield）逻辑控制 */
class PlayFieldController {
public:
    void init(const GameModel* gameModel);
    /** 在 GameView 创建后调用，绑定视图（如主牌区 Layer、牌节点等） */
    void initView(GameViewScene* gameView);
    /** 处理用户点击桌面卡片 */
    void handleCardClick(int cardId);

private:
    const GameModel* _gameModel = nullptr;
    GameViewScene* _gameView = nullptr;

    void replaceTrayWithPlayFieldCard(int cardId);
};

} // namespace playcard
