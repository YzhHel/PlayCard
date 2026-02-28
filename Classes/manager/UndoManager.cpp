#include "UndoManager.h"

namespace playcard {

void UndoManager::init(const GameModel* gameModel) {
    _gameModel = gameModel;
}

void UndoManager::pushUndoAction(const std::function<void()>& action) {
    if (action) {
        _undoStack.push_back(action);
    }
}

void UndoManager::undoLast() {
    if (_undoStack.empty()) {
        return;
    }
    auto action = _undoStack.back();
    _undoStack.pop_back();
    if (action) {
        action();
    }
}

bool UndoManager::canUndo() const {
    return !_undoStack.empty();
}

} // namespace playcard
