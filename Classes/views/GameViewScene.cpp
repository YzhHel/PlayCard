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

        // 目标：左边显示多余牌（除顶牌），右边只显示一张顶牌，并在顶牌右侧放“回退”按钮
        // 为避免“挤”，把堆牌区划分成：左侧叠牌区域 + 右侧顶牌/按钮区域
        const float leftRegionMinX = pileW * 0.12f;
        const float leftRegionMaxX = pileW * 0.42f;
        const float topCardX = pileW * 0.70f;

        CardViewSceneItem* topCardNode = nullptr;

        const int n = static_cast<int>(stackPlacements.size());
        const int extraCount = std::max(0, n - 1);
        if (extraCount > 0) {
            const float maxSpacing = 18.0f;
            const float availableLeftWidth = std::max(0.0f, leftRegionMaxX - leftRegionMinX);
            const float spacing = (extraCount <= 1) ? 0.0f : std::min(maxSpacing, availableLeftWidth / (extraCount - 1));

            for (int i = 0; i < extraCount; ++i) {
                const auto& c = stackPlacements[i];
                auto card = CardViewSceneItem::create(c.cardFace, c.cardSuit);
                if (!card) {
                    continue;
                }
                card->setPosition(Vec2(leftRegionMinX + i * spacing, centerY));
                card->setLocalZOrder(i);
                _pileLayer->addChild(card);
                card->setClickCallback([card]() {});
                card->setIsShowUp(0);
            }
        }

        // 顶牌（最后一张）放右侧
        {
            const auto& c = stackPlacements.back();
            auto card = CardViewSceneItem::create(c.cardFace, c.cardSuit);
            if (card) {
                card->setPosition(Vec2(topCardX, centerY));
                card->setLocalZOrder(1000);
                _pileLayer->addChild(card);
                card->setClickCallback([card]() {});
                card->setIsShowUp(1);
                topCardNode = card;
            }
        }

        // 回退按钮：放在顶牌右侧
        if (topCardNode) {
            auto label = Label::createWithSystemFont("回退", "", 22);
            label->setTextColor(Color4B::WHITE);
            auto item = MenuItemLabel::create(label, [](Ref*) {
                CCLOG("Undo/Back clicked");
            });

            auto menu = Menu::create(item, nullptr);
            menu->setPosition(Vec2::ZERO);
            _pileLayer->addChild(menu, 2000);

            const float cardW = topCardNode->getContentSize().width;
            const float gap = 16.0f;
            const float btnHalfW = item->getContentSize().width / 2.0f;
            float btnX = topCardNode->getPositionX() + cardW / 2.0f + gap + btnHalfW;
            btnX = std::min(btnX, pileW - btnHalfW - 12.0f);
            item->setPosition(Vec2(btnX, topCardNode->getPositionY()));
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
