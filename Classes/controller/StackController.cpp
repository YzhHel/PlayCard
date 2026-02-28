#include "StackController.h"
#include "views/GameViewScene.h"
#include "views/PlayFieldView.h"
#include "views/CardViewSceneItem.h"
#include "manager/UndoManager.h"
#include "cocos2d.h"

USING_NS_CC;

namespace playcard {

void StackController::init(const GameModel* gameModel, UndoManager* undoManager) {
    _gameModel = gameModel;
    _undoManager = undoManager;
}

void StackController::initView(GameViewScene* gameView) {
    _gameView = gameView;
    if (!_gameView) {
        return;
    }
    _gameView->bindStackController(this);

    auto pileLayer = _gameView->getPileLayer();
    if (!pileLayer) {
        return;
    }

    _hiddenCards.clear();
    _topCard = nullptr;
    _undoItem = nullptr;

    // 1. 扫描堆牌区子节点，找出所有牌与回退按钮
    auto children = pileLayer->getChildren();
    std::vector<CardViewSceneItem*> allCards;
    for (auto child : children) {
        if (auto card = dynamic_cast<CardViewSceneItem*>(child)) {
            allCards.push_back(card);
        } else if (auto menu = dynamic_cast<Menu*>(child)) {
            for (auto mChild : menu->getChildren()) {
                if (auto item = dynamic_cast<MenuItemLabel*>(mChild)) {
                    _undoItem = item;
                }
            }
        }
    }

    if (allCards.empty()) {
        return;
    }

    // 2. 以 X 坐标最大者作为当前顶牌，其余为左侧叠牌
    _topCard = allCards.front();
    for (auto c : allCards) {
        if (c->getPositionX() > _topCard->getPositionX()) {
            _topCard = c;
        }
    }

    for (auto c : allCards) {
        if (c != _topCard) {
            _hiddenCards.push_back(c);
        }
    }

    if (!_hiddenCards.empty()) {
        _leftSlotPos = _hiddenCards.front()->getPosition();
    } else if (_topCard) {
        // 没有隐藏牌时，左槽暂时与顶牌重合
        _leftSlotPos = _topCard->getPosition();
    }
    if (_topCard) {
        _centerSlotPos = _topCard->getPosition();
    }

    // 3. 重新绑定交互：左侧叠牌点击 -> 翻牌；回退按钮 -> 撤销
    for (auto c : _hiddenCards) {
        c->setClickCallback([this]() {
            this->onHiddenStackClicked();
        });
    }

    if (_undoItem) {
        _undoItem->setCallback([this](Ref*) {
            this->onUndoClicked();
        });
    }
}

void StackController::onHiddenStackClicked() {
    if (!_topCard || _hiddenCards.empty()) {
        return;
    }

    // 取当前左侧叠牌中“最上面”的一张（向量末尾）
    CardViewSceneItem* newTop = _hiddenCards.back();
    CardViewSceneItem* oldTop = _topCard;

    if (!newTop || !oldTop || newTop == oldTop) {
        return;
    }

    // 前向操作：newTop 从左槽移到中槽，oldTop 从中槽移到左槽
    _hiddenCards.pop_back();
    _hiddenCards.push_back(oldTop);
    _topCard = newTop;

    newTop->stopAllActions();
    oldTop->stopAllActions();
    newTop->runAction(MoveTo::create(0.2f, _centerSlotPos));
    oldTop->runAction(MoveTo::create(0.2f, _leftSlotPos));

    // 记录撤销：再次交换回来
    if (_undoManager) {
        _undoManager->pushUndoAction([this, oldTop, newTop]() {
            if (!oldTop || !newTop) {
                return;
            }

            newTop->stopAllActions();
            oldTop->stopAllActions();
            newTop->runAction(MoveTo::create(0.2f, _leftSlotPos));
            oldTop->runAction(MoveTo::create(0.2f, _centerSlotPos));

            // 还原内部状态：oldTop 重新成为顶牌
            auto it = std::find(_hiddenCards.begin(), _hiddenCards.end(), oldTop);
            if (it != _hiddenCards.end()) {
                _hiddenCards.erase(it);
            }
            _hiddenCards.push_back(newTop);
            _topCard = oldTop;
        });
    }
}

void StackController::handlePlayfieldCardClick(int cardId) {
    if (!_gameView || !_topCard || !_gameModel) {
        return;
    }
    if (cardId < 0 || cardId >= static_cast<int>(_gameModel->playfield.size())) {
        return;
    }

    auto mainLayer = dynamic_cast<PlayFieldView*>(_gameView->getMainLayer());
    auto pileLayer = _gameView->getPileLayer();
    if (!mainLayer || !pileLayer) {
        return;
    }

    CardViewSceneItem* deskCard = mainLayer->getCardNode(cardId);
    if (!deskCard) {
        return;
    }

    int deskFace = deskCard->getFace();
    int topFace = _topCard->getFace();

    // 需求2：桌面牌和手牌区顶部牌点数差 1 才能匹配
    if (std::abs(deskFace - topFace) != 1) {
        return;
    }

    CardViewSceneItem* oldTop = _topCard;
    CardViewSceneItem* newTop = deskCard;

    // 记录桌面牌原始位置（在主牌区坐标系下）
    Vec2 deskOriginalPos = deskCard->getPosition();

    // 将桌面牌从主牌区移到堆牌区（保持当前世界坐标，再在堆牌区内播放动画）
    Vec2 worldPos = mainLayer->convertToWorldSpace(deskOriginalPos);
    Vec2 startPosInPile = pileLayer->convertToNodeSpace(worldPos);

    deskCard->retain();
    mainLayer->removeChild(deskCard, false);
    pileLayer->addChild(deskCard);
    deskCard->release();
    deskCard->setPosition(startPosInPile);

    // 前向动画：桌面牌飞到中槽，原顶牌飞到左槽
    deskCard->stopAllActions();
    oldTop->stopAllActions();
    deskCard->runAction(MoveTo::create(0.2f, _centerSlotPos));
    oldTop->runAction(MoveTo::create(0.2f, _leftSlotPos));

    // 更新内部手牌状态
    _hiddenCards.push_back(oldTop);
    _topCard = newTop;

    // 桌面视图不再响应这张牌的点击
    mainLayer->unregisterCard(cardId);

    // 记录撤销操作：把 newTop 送回桌面，把 oldTop 送回中槽
    if (_undoManager) {
        _undoManager->pushUndoAction([this, mainLayer, pileLayer, cardId, deskCard, oldTop, newTop, deskOriginalPos]() {
            if (!deskCard || !oldTop || !newTop || !mainLayer || !pileLayer) {
                return;
            }

            // 将 newTop（即 deskCard）从堆牌区移回主牌区原位置
            Vec2 worldPosNow = pileLayer->convertToWorldSpace(deskCard->getPosition());
            Vec2 posInMain = mainLayer->convertToNodeSpace(worldPosNow);

            deskCard->retain();
            pileLayer->removeChild(deskCard, false);
            mainLayer->addChild(deskCard);
            deskCard->release();
            deskCard->setPosition(deskOriginalPos);

            // 原顶牌回到中槽
            oldTop->stopAllActions();
            oldTop->runAction(MoveTo::create(0.2f, _centerSlotPos));

            // 更新内部手牌状态
            auto it = std::find(_hiddenCards.begin(), _hiddenCards.end(), oldTop);
            if (it != _hiddenCards.end()) {
                _hiddenCards.erase(it);
            }
            _topCard = oldTop;

            // 重新在桌面注册该牌的点击
            mainLayer->registerCard(cardId, deskCard);
        });
    }
}

void StackController::onUndoClicked() {
    if (_undoManager) {
        _undoManager->undoLast();
    }
}

} // namespace playcard
