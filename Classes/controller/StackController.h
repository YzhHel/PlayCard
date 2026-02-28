#pragma once

#include "configs/models/GameModel.h"
#include "cocos2d.h"

class GameViewScene;
class CardViewSceneItem;

namespace playcard {

class UndoManager;

/** 堆牌区（Stack）逻辑控制 */
class StackController {
public:
    void init(const GameModel* gameModel, UndoManager* undoManager);
    /** 在 GameView 创建后调用，绑定堆牌区视图，并接管交互 */
    void initView(GameViewScene* gameView);

    /** 由桌面控制器转发：用户点击了桌面上的某张牌（cardId 为 Playfield 中的索引） */
    void handlePlayfieldCardClick(int cardId);

private:
    const GameModel* _gameModel = nullptr;
    GameViewScene* _gameView = nullptr;
    UndoManager* _undoManager = nullptr;

    // 手牌区布局与节点引用
    std::vector<CardViewSceneItem*> _hiddenCards;  // 左侧备用牌（横向折叠）
    CardViewSceneItem* _topCard = nullptr;         // 中间显示的顶牌
    float _fanBaseX = 0.0f;                        // 备用牌横向折叠起始 X
    float _fanOffsetX = 28.0f;                     // 备用牌每张横向偏移
    cocos2d::Vec2 _centerSlotPos;                 // 中间顶牌槽位置
    cocos2d::MenuItemLabel* _undoItem = nullptr;  // 右侧回退按钮

    // 手牌区自身交互
    void onHiddenStackClicked();
    void onUndoClicked();
};

} // namespace playcard
