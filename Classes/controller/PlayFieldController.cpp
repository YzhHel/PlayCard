#include "PlayFieldController.h"
#include "views/GameViewScene.h"
#include "views/PlayFieldView.h"
#include "cocos2d.h"

USING_NS_CC;

namespace playcard {

void PlayFieldController::init(const GameModel* gameModel) {
    _gameModel = gameModel;
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
    // TODO: 检查 cardId 对应卡片是否满足移动条件（根据 _gameModel 规则）
    // 目前先直接调用替换逻辑占位
    replaceTrayWithPlayFieldCard(cardId);
}

void PlayFieldController::replaceTrayWithPlayFieldCard(int cardId) {
    // TODO:
    // - 记录撤销操作（通过 UndoManager）
    // - 更新 _gameModel 中对应卡片位置/状态
    // - 通过 _gameView 执行动画（从牌堆移到桌面等）
    CCLOG("replaceTrayWithPlayFieldCard, cardId=%d", cardId);
}

} // namespace playcard
