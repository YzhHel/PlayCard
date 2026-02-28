#pragma once

#include "cocos2d.h"
#include <functional>
#include <unordered_map>

class CardViewSceneItem;

/** 主牌区视图：负责桌面牌的点击转发给上层控制器 */
class PlayFieldView : public cocos2d::LayerColor {
public:
    static PlayFieldView* create(const cocos2d::Color4B& color, float width, float height);

    using CardClickCallback = std::function<void(int cardId)>;

    void setCardClickCallback(CardClickCallback cb);

    /** 注册一张桌面牌及其 cardId，内部为该牌设置点击回调 */
    void registerCard(int cardId, CardViewSceneItem* card);

protected:
    bool initWithColor(const cocos2d::Color4B& color, float width, float height);

private:
    CardClickCallback _onCardClickCallback;
    std::unordered_map<int, CardViewSceneItem*> _cards;

public:
    /** 通过 cardId 获取对应的牌节点（如果存在） */
    CardViewSceneItem* getCardNode(int cardId) const;

    /** 取消某张桌面牌的点击注册（例如移动到手牌区后） */
    void unregisterCard(int cardId);
};

