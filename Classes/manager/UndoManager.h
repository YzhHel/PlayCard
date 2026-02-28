#pragma once

#include "configs/models/GameModel.h"

namespace playcard {

/** 撤销/重做管理，与当前关卡游戏模型关联 */
class UndoManager {
public:
    void init(const GameModel* gameModel);
    // 可扩展：undo(), redo(), pushState() 等
private:
    const GameModel* _gameModel = nullptr;
};

} // namespace playcard
