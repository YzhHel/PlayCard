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

    // 1. 根据堆牌区尺寸，重新计算三个槽位的坐标（与 GameViewScene::renderStack 保持一致）
    const float pileW = pileLayer->getContentSize().width;
    const float pileH = pileLayer->getContentSize().height;
    const float centerY = pileH / 2.0f;
    const float sectionWidth = pileW / 3.0f;
    const float leftX   = sectionWidth * 0.5f;   // 左区中心
    const float centerX = sectionWidth * 1.5f;   // 中区中心
    // const float rightX  = sectionWidth * 2.5f; // 右区中心（仅用于按钮，不在此处记录）

    _leftSlotPos   = Vec2(leftX, centerY);
    _centerSlotPos = Vec2(centerX, centerY);

    // 2. 扫描堆牌区子节点，找出所有牌与回退按钮
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

    // 3. 以 X 坐标最大者作为当前顶牌，其余为左侧叠牌
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

    // 4. 重新绑定交互：左侧叠牌点击 -> 翻牌；回退按钮 -> 撤销
    for (auto c : _hiddenCards) {
        // 备用牌：不需要“顶牌限制”，点击任意一张都直接翻到顶牌位置
        c->setEnforceTopMost(false);
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
    // 备用牌堆为空则无操作
    if (_hiddenCards.empty()) {
        return;
    }

    // 取当前左侧叠牌中“最上面”的一张（向量末尾）
    CardViewSceneItem* newTop = _hiddenCards.back();
    CardViewSceneItem* oldTop = _topCard;

    if (!newTop) {
        return;
    }

    // 前向操作：newTop 从左槽移到中槽，覆盖在当前顶牌之上；
    // 旧顶牌隐藏，仅保留为“历史顶牌”以便撤销时恢复
    _hiddenCards.pop_back();
    _topCard = newTop;

    if (oldTop) {
        oldTop->setVisible(false);
    }
    newTop->setVisible(true);
    // 需求：翻到顶牌后正面朝上
    newTop->setIsShowUp(1);
    newTop->stopAllActions();
    newTop->runAction(MoveTo::create(0.2f, _centerSlotPos));

    // 记录撤销：将 newTop 从顶牌区域送回左侧备用堆
    if (_undoManager) {
        _undoManager->pushUndoAction([this, oldTop, newTop]() {
            if (!newTop) {
                return;
            }

            newTop->stopAllActions();
            newTop->runAction(MoveTo::create(0.2f, _leftSlotPos));

            // 还原内部状态：oldTop 重新作为当前顶牌，newTop 回到备用堆
            _hiddenCards.push_back(newTop);
            _topCard = oldTop;

            if (oldTop) {
                oldTop->setVisible(true);
            }
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

    // 前向动画：桌面牌飞到中槽，覆盖在当前顶牌之上（顶牌区域为叠放容器）
    deskCard->stopAllActions();
    deskCard->runAction(MoveTo::create(0.2f, _centerSlotPos));

    // 更新当前顶牌指针与可见性：旧顶牌隐藏，新顶牌显示
    if (oldTop) {
        oldTop->setVisible(false);
    }
    newTop->setVisible(true);
    _topCard = newTop;

    // 桌面视图不再响应这张牌的点击
    mainLayer->unregisterCard(cardId);

    // 记录撤销操作：把 newTop 送回桌面，把 oldTop 送回中槽
    if (_undoManager) {
        _undoManager->pushUndoAction([this, mainLayer, pileLayer, cardId, deskCard, oldTop, newTop, deskOriginalPos]() {
            if (!deskCard || !oldTop || !newTop || !mainLayer || !pileLayer) {
                return;
            }

            // 将 newTop（即 deskCard）从堆牌区移回主牌区
            deskCard->retain();
            pileLayer->removeChild(deskCard, false);
            mainLayer->addChild(deskCard);
            deskCard->release();
            // 使用 MoveTo 反向平移回原始坐标（桌面坐标系）
            deskCard->stopAllActions();
            deskCard->runAction(MoveTo::create(0.2f, deskOriginalPos));

            // 撤销匹配：恢复旧顶牌为当前顶牌并重新显示
            _topCard = oldTop;
            if (oldTop) {
                oldTop->setVisible(true);
            }

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
