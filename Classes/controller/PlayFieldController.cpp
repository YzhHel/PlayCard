#include "PlayFieldController.h"
#include "StackController.h"
#include "views/GameViewScene.h"
#include "views/PlayFieldView.h"
#include "manager/UndoManager.h"
#include "cocos2d.h"

USING_NS_CC;

namespace playcard {

void PlayFieldController::init(const GameModel* gameModel, StackController* stackController, UndoManager* undoManager) {
    _gameModel = gameModel;
    _stackController = stackController;
    _undoManager = undoManager;
}

void PlayFieldController::initView(GameViewScene* gameView) {
    _gameView = gameView;
    if (_gameView) {
        _gameView->bindPlayFieldController(this);

        auto playLayer = dynamic_cast<PlayFieldView*>(_gameView->getMainLayer());
        if (playLayer) {
            playLayer->setCardClickCallback([this](int cardId) {
                this->handleCardClick(cardId);
            });
        }
    }
}

void PlayFieldController::handleCardClick(int cardId) {
    // 桌面牌点击后，由 StackController 处理与手牌顶牌的匹配逻辑
    if (_stackController) {
        _stackController->handlePlayfieldCardClick(cardId);
    }
}

void PlayFieldController::replaceTrayWithPlayFieldCard(int /*cardId*/) {
    // 逻辑已迁移到 StackController::handlePlayfieldCardClick
}

} // namespace playcard
