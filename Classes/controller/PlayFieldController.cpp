#include "PlayFieldController.h"
#include "views/GameViewScene.h"

namespace playcard {

void PlayFieldController::init(const GameModel* gameModel) {
    _gameModel = gameModel;
}

void PlayFieldController::initView(GameViewScene* gameView) {
    _gameView = gameView;
    if (_gameView) {
        _gameView->bindPlayFieldController(this);
    }
}

} // namespace playcard
