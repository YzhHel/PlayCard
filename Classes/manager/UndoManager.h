#pragma once

#include "configs/models/GameModel.h"
#include <functional>
#include <vector>

namespace playcard {

/** 撤销/重做管理，与当前关卡游戏模型关联 */
class UndoManager {
public:
    void init(const GameModel* gameModel);

    /** 记录一条撤销操作（后进先出） */
    void pushUndoAction(const std::function<void()>& action);

    /** 撤销最近一次操作（如果有的话） */
    void undoLast();

    /** 是否还有可撤销的记录 */
    bool canUndo() const;

private:
    const GameModel* _gameModel = nullptr;
    std::vector<std::function<void()>> _undoStack;
};

} // namespace playcard
