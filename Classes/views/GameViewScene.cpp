#include "GameViewScene.h"
#include "GameViewScene.h"
#include "CardViewSceneItem.h"
#include "json/document.h"

USING_NS_CC;

#define MainCardArea_Width 540.0f
#define MainCardArea_Height 750.0f
#define CardDeckArea_Width 540.0f
#define CardDeckArea_Height 290.0f

Scene* GameViewScene::createScene()
{
    return GameViewScene::create();
}

bool GameViewScene::init()
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

    // load configuration JSON from resources (res/res/playfield.json) and parse with RapidJSON
    std::string cfgPath = "res/res/playfield.json";
    std::string raw = "";
    if (FileUtils::getInstance()->isFileExist(cfgPath)) {
        raw = FileUtils::getInstance()->getStringFromFile(cfgPath);
    } else {
        CCLOG("playfield.json not found at %s", cfgPath.c_str());
    }

    rapidjson::Document doc;
    if (raw.empty()) {
        CCLOG("playfield.json is empty or missing: %s", cfgPath.c_str());
    } else {
        doc.Parse(raw.c_str());
        if (doc.HasParseError() || !doc.IsObject()) {
            CCLOG("RapidJSON parse failed for %s, error offset=%zu", cfgPath.c_str(), doc.GetErrorOffset());
        } else {
            // Playfield
            if (doc.HasMember("Playfield") && doc["Playfield"].IsArray()) {
                const rapidjson::Value& arr = doc["Playfield"];
                for (rapidjson::SizeType i = 0; i < arr.Size(); ++i) {
                    const rapidjson::Value& it = arr[i];
                    int face = it.HasMember("CardFace") ? it["CardFace"].GetInt() : 0;
                    int suit = it.HasMember("CardSuit") ? it["CardSuit"].GetInt() : 0;
                    float x = 0, y = 0;
                    if (it.HasMember("Position") && it["Position"].IsObject()) {
                        const rapidjson::Value& p = it["Position"];
                        x = p.HasMember("x") ? p["x"].GetFloat() : 0;
                        y = p.HasMember("y") ? p["y"].GetFloat() : 0;
                    }
                    auto card = CardViewSceneItem::create(face, suit);
                    if (card) {
                        card->setPosition(Vec2(x, y));
                        mainLayer->addChild(card);
                        card->updateTouchSize(Vec2(x, y));
                    }
                }
            }

            // Stack
            if (doc.HasMember("Stack") && doc["Stack"].IsArray()) {
                const rapidjson::Value& arr = doc["Stack"];
                for (rapidjson::SizeType i = 0; i < arr.Size(); ++i) {
                    const rapidjson::Value& it = arr[i];
                    int face = it.HasMember("CardFace") ? it["CardFace"].GetInt() : 0;
                    int suit = it.HasMember("CardSuit") ? it["CardSuit"].GetInt() : 0;
                    float x = 0, y = 0;
                    if (it.HasMember("Position") && it["Position"].IsObject()) {
                        const rapidjson::Value& p = it["Position"];
                        x = p.HasMember("x") ? p["x"].GetFloat() : 0;
                        y = p.HasMember("y") ? p["y"].GetFloat() : 0;
                    }
                    auto card = CardViewSceneItem::create(face, suit);
                    if (card) {
                        card->setPosition(Vec2(x + pileWidth/2.0f, y + pileHeight/2.0f));
                        pileLayer->addChild(card);
                        card->updateTouchSize(Vec2(x + pileWidth / 2.0f, y + pileHeight / 2.0f));
                    }
                }
            }
        }
    }

    return true;
}
