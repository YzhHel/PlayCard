#pragma once

#include "cocos2d.h"
#include <functional>

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
};

