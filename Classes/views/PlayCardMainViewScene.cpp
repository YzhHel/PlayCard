#include "PlayCardMainViewScene.h"
#include "controller/GameController.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace ui;

// 全局持有一个 GameController，保证其生命周期覆盖整个游戏过程
namespace {
    std::unique_ptr<playcard::GameController> gGameController;
}

#define MainCardArea_Width 1080.0f
#define MainCardArea_Height 1500.0f
#define CardDeckArea_Width 1080.0f
#define CardDeckArea_Height 580.0f

Scene* PlayCardMainViewScene::createScene()
{
    return PlayCardMainViewScene::create();
}

PlayCardMainViewScene::~PlayCardMainViewScene() = default;

bool PlayCardMainViewScene::init()
{
    if (!Scene::init()) {
        return false;
    }

    // 初始化全局 GameController 单例（如尚未创建）
    if (!gGameController) {
        gGameController = std::make_unique<playcard::GameController>();
    }
    _gameController = gGameController.get();

    setLevelSelectedCallback([this](int levelId) {
        if (_gameController) {
            _gameController->startGame(levelId);
        }
    });

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    const float mainWidth = MainCardArea_Width;
    const float mainHeight = MainCardArea_Height;
    const float pileWidth = CardDeckArea_Width;
    const float pileHeight = CardDeckArea_Height;
    const float totalHeight = mainHeight + pileHeight;

    float startX = origin.x + (visibleSize.width - mainWidth) / 2.0f;
    float startY = origin.y + (visibleSize.height - totalHeight) / 2.0f;

    Color4B tanColor(210, 180, 140, 255);
    auto mainLayer = LayerColor::create(tanColor, mainWidth, mainHeight);
    mainLayer->setPosition(Vec2(startX, startY + pileHeight));
    this->addChild(mainLayer, 1);

    Color4B purpleColor(128, 0, 128, 255);
    auto pileLayer = LayerColor::create(purpleColor, pileWidth, pileHeight);
    pileLayer->setPosition(Vec2(startX, startY));
    this->addChild(pileLayer, 1);

    buildLevelSelectionUI();
    return true;
}

void PlayCardMainViewScene::buildLevelSelectionUI()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    float cx = origin.x + visibleSize.width / 2.0f;
    float cy = origin.y + visibleSize.height / 2.0f;

    auto label = Label::createWithSystemFont("选择关卡", "", 48);
    label->setPosition(Vec2(cx, cy + 120.0f));
    this->addChild(label, 10);

    // 关卡按钮：1、2、3，点击时触发关卡选择事件
    for (int levelId = 1; levelId <= 3; ++levelId) {
        auto btn = Button::create();
        btn->setTitleText("关卡 " + std::to_string(levelId));
        btn->setTitleFontSize(40);
        btn->setScale(1.5f);
        btn->setPosition(Vec2(cx - 180.0f + (levelId - 1) * 180.0f, cy));
        btn->addClickEventListener([this, levelId](Ref*) {
            onLevelButtonClicked(levelId);
        });
        this->addChild(btn, 10);
    }
}

void PlayCardMainViewScene::onLevelButtonClicked(int levelId)
{
    if (_onLevelSelected) {
        _onLevelSelected(levelId);
    }
}
