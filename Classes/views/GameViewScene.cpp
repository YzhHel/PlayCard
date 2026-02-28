#include "GameViewScene.h"
#include "CardViewSceneItem.h"
#include "PlayFieldView.h"
#include "controller/PlayFieldController.h"
#include "controller/StackController.h"
#include "json/document.h"
#include <algorithm>

USING_NS_CC;

#define MainCardArea_Width 540.0f
#define MainCardArea_Height 750.0f
#define CardDeckArea_Width 540.0f
#define CardDeckArea_Height 290.0f

Scene* GameViewScene::createScene()
{
    return GameViewScene::create();
}

GameViewScene* GameViewScene::createWithGameModel(const playcard::GameModel* model)
{
    auto p = new (std::nothrow) GameViewScene();
    if (p && p->initWithGameModel(model)) {
        p->autorelease();
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

bool GameViewScene::init()
{
    return initWithGameModel(nullptr);
}

bool GameViewScene::initWithGameModel(const playcard::GameModel* model)
{
    if (!Scene::init()) {
        return false;
    }
    _gameModel = model;

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    const float mainWidth = MainCardArea_Width;
    const float mainHeight = MainCardArea_Height;
    const float pileWidth = CardDeckArea_Width;
    const float pileHeight = CardDeckArea_Height;

    float startX = origin.x + (visibleSize.width - mainWidth) / 2.0f;
    float startY = origin.y + (visibleSize.height - (mainHeight + pileHeight)) / 2.0f;

    Color4B tanColor(210, 180, 140, 255);
    _mainLayer = PlayFieldView::create(tanColor, mainWidth, mainHeight);
    _mainLayer->setPosition(Vec2(startX, startY + pileHeight));
    this->addChild(_mainLayer, 1);

    Color4B purpleColor(128, 0, 128, 255);
    _pileLayer = LayerColor::create(purpleColor, pileWidth, pileHeight);
    _pileLayer->setPosition(Vec2(startX, startY));
    this->addChild(_pileLayer, 1);

    auto renderStack = [&](const std::vector<playcard::CardPlacement>& stackPlacements) {
        if (!_pileLayer) {
            return;
        }
        if (stackPlacements.empty()) {
            return;
        }

        const float pileW = _pileLayer->getContentSize().width;
        const float pileH = _pileLayer->getContentSize().height;
        const float centerY = pileH / 2.0f;

        // 将堆牌区等分为 3 个区域：左（未显示的牌叠放）、中（顶牌）、右（回退按钮）
        const float sectionWidth = pileW / 3.0f;
        const float leftX   = sectionWidth * 0.5f;   // 左区中心
        const float centerX = sectionWidth * 1.5f;   // 中区中心
        const float rightX  = sectionWidth * 2.5f;   // 右区中心

        CardViewSceneItem* topCardNode = nullptr;

        const int n = static_cast<int>(stackPlacements.size());
        const int extraCount = std::max(0, n - 1); // 除去顶牌的数量

        // 1. 左侧：未显示的牌，全部重叠在同一个位置
        for (int i = 0; i < extraCount; ++i) {
            const auto& c = stackPlacements[i];
            auto card = CardViewSceneItem::create(c.cardFace, c.cardSuit);
            if (!card) {
                continue;
            }

            card->setPosition(Vec2(leftX, centerY));
            card->setLocalZOrder(i);
            _pileLayer->addChild(card);
            card->setClickCallback([card]() {});
            card->setIsShowUp(0); // 全部背面
        }

        // 2. 中间：顶牌（最后一张），居中显示
        {
            const auto& c = stackPlacements.back();
            auto card = CardViewSceneItem::create(c.cardFace, c.cardSuit);
            if (card) {
                card->setPosition(Vec2(centerX, centerY));
                card->setLocalZOrder(1000);
                _pileLayer->addChild(card);
                card->setClickCallback([card]() {});
                card->setIsShowUp(1); // 顶牌为明牌
                topCardNode = card;
            }
        }

        // 3. 右侧：回退按钮，居右区域中心
        if (topCardNode) {
            auto label = Label::createWithSystemFont("回退", "", 22);
            label->setTextColor(Color4B::WHITE);
            auto item = MenuItemLabel::create(label, [](Ref*) {
                CCLOG("Undo/Back clicked");
            });

            auto menu = Menu::create(item, nullptr);
            menu->setPosition(Vec2::ZERO);
            _pileLayer->addChild(menu, 2000);

            item->setPosition(Vec2(rightX, centerY));
        }
    };

    if (_gameModel) {
        auto playFieldView = dynamic_cast<PlayFieldView*>(_mainLayer);
        if (playFieldView) {
            for (int i = 0; i < static_cast<int>(_gameModel->playfield.size()); ++i) {
                const auto& c = _gameModel->playfield[i];
                auto card = CardViewSceneItem::create(c.cardFace, c.cardSuit);
                if (card) {
                    card->setPosition(Vec2(c.position.x, c.position.y));
                    _mainLayer->addChild(card);
                    playFieldView->registerCard(i, card);
                }
            }
        }
        renderStack(_gameModel->stack);
    } else {
        std::string cfgPath = "res/res/playfield.json";
        std::string raw;
        if (FileUtils::getInstance()->isFileExist(cfgPath)) {
            raw = FileUtils::getInstance()->getStringFromFile(cfgPath);
        }
        if (!raw.empty()) {
            rapidjson::Document doc;
            doc.Parse(raw.c_str());
            if (!doc.HasParseError() && doc.IsObject()) {
                if (doc.HasMember("Playfield") && doc["Playfield"].IsArray()) {
                    const auto& arr = doc["Playfield"];
                    auto playFieldView = dynamic_cast<PlayFieldView*>(_mainLayer);
                    for (rapidjson::SizeType i = 0; i < arr.Size(); ++i) {
                        const auto& it = arr[i];
                        int face = it.HasMember("CardFace") ? it["CardFace"].GetInt() : 0;
                        int suit = it.HasMember("CardSuit") ? it["CardSuit"].GetInt() : 0;
                        float x = 0, y = 0;
                        if (it.HasMember("Position") && it["Position"].IsObject()) {
                            const auto& p = it["Position"];
                            x = p.HasMember("x") ? p["x"].GetFloat() : 0;
                            y = p.HasMember("y") ? p["y"].GetFloat() : 0;
                        }
                        auto card = CardViewSceneItem::create(face, suit);
                        if (card) {
                            card->setPosition(Vec2(x, y));
                            _mainLayer->addChild(card);
                            if (playFieldView) {
                                playFieldView->registerCard(static_cast<int>(i), card);
                            }
                        }
                    }
                }
                if (doc.HasMember("Stack") && doc["Stack"].IsArray()) {
                    const auto& arr = doc["Stack"];
                    std::vector<playcard::CardPlacement> stackPlacements;
                    stackPlacements.reserve(arr.Size());
                    for (rapidjson::SizeType i = 0; i < arr.Size(); ++i) {
                        const auto& it = arr[i];
                        int face = it.HasMember("CardFace") ? it["CardFace"].GetInt() : 0;
                        int suit = it.HasMember("CardSuit") ? it["CardSuit"].GetInt() : 0;
                        playcard::CardPlacement c;
                        c.cardFace = face;
                        c.cardSuit = suit;
                        stackPlacements.push_back(c);
                    }
                    renderStack(stackPlacements);
                }
            }
        }
    }

    setupInitialLayout();
    return true;
}

void GameViewScene::setupInitialLayout()
{
    // 初始布局已在 init 中完成（主牌区/堆牌区位置与牌位置）
    // 可在此扩展：得分区、按钮等
}

void GameViewScene::playEntranceAnimation()
{
    // 播放入场动画占位：例如淡入、牌飞入等
    if (_mainLayer) {
        _mainLayer->setOpacity(0);
        _mainLayer->runAction(FadeIn::create(0.3f));
    }
    if (_pileLayer) {
        _pileLayer->setOpacity(0);
        _pileLayer->runAction(FadeIn::create(0.3f));
    }
}

void GameViewScene::bindPlayFieldController(playcard::PlayFieldController* ctrl)
{
    _playFieldController = ctrl;
}

void GameViewScene::bindStackController(playcard::StackController* ctrl)
{
    _stackController = ctrl;
}
