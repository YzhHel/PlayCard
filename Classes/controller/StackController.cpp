#include "StackController.h"
#include "views/GameViewScene.h"

namespace playcard {

void StackController::init(const GameModel* gameModel) {
    _gameModel = gameModel;
}

void StackController::initView(GameViewScene* gameView) {
    _gameView = gameView;
    if (_gameView) {
        _gameView->bindStackController(this);
    }
}

} // namespace playcard
