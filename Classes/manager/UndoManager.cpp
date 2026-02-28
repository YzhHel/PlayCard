#include "UndoManager.h"

namespace playcard {

void UndoManager::init(const GameModel* gameModel) {
    _gameModel = gameModel;
}

} // namespace playcard
