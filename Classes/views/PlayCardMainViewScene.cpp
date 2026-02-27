#include "PlayCardMainViewScene.h"

USING_NS_CC;

#define MainCardArea_Width 540.0f
#define MainCardArea_Height 750.0f
#define CardDeckArea_Width 540.0f
#define CardDeckArea_Height 290.0f

Scene* PlayCardMainViewScene::createScene()
{
    return PlayCardMainViewScene::create();
}

bool PlayCardMainViewScene::init()
{
    if (!Scene::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    const float mainWidth = MainCardArea_Width;
    const float mainHeight = MainCardArea_Height;
    const float pileWidth = CardDeckArea_Width;
    const float pileHeight = CardDeckArea_Height;
    
    const float totalHeight = mainHeight + pileHeight;

    // center horizontally, vertically center the combined block
    float startX = origin.x + (visibleSize.width - mainWidth) / 2.0f;
    float startY = origin.y + (visibleSize.height - totalHeight) / 2.0f;

    // 主牌区（土黄色）
    Color4B tanColor(210, 180, 140, 255); // 土黄色
    auto mainLayer = LayerColor::create(tanColor, mainWidth, mainHeight);
    mainLayer->setPosition(Vec2(startX, startY + pileHeight ));
    this->addChild(mainLayer, 1);

    // 堆牌区（紫色）
    Color4B purpleColor(128, 0, 128, 255); // 紫色
    auto pileLayer = LayerColor::create(purpleColor, pileWidth, pileHeight);
    pileLayer->setPosition(Vec2(startX, startY));
    this->addChild(pileLayer, 1);

    return true;
}
