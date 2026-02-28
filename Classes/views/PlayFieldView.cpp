#include "PlayFieldView.h"
#include "CardViewSceneItem.h"

USING_NS_CC;

PlayFieldView* PlayFieldView::create(const Color4B& color, float width, float height)
{
    auto p = new (std::nothrow) PlayFieldView();
    if (p && p->initWithColor(color, width, height)) {
        p->autorelease();
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

bool PlayFieldView::initWithColor(const Color4B& color, float width, float height)
{
    if (!LayerColor::initWithColor(color, width, height)) {
        return false;
    }
    return true;
}

void PlayFieldView::setCardClickCallback(CardClickCallback cb)
{
    _onCardClickCallback = std::move(cb);
}

void PlayFieldView::registerCard(int cardId, CardViewSceneItem* card)
{
    if (!card) {
        return;
    }
    card->setClickCallback([this, cardId]() {
        if (_onCardClickCallback) {
            _onCardClickCallback(cardId);
        }
    });
    _cards[cardId] = card;
}

CardViewSceneItem* PlayFieldView::getCardNode(int cardId) const
{
    auto it = _cards.find(cardId);
    if (it != _cards.end()) {
        return it->second;
    }
    return nullptr;
}

void PlayFieldView::unregisterCard(int cardId)
{
    auto it = _cards.find(cardId);
    if (it != _cards.end()) {
        _cards.erase(it);
    }
}

