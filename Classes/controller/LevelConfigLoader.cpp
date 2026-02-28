#include "LevelConfigLoader.h"
#include "cocos2d.h"
#include "json/document.h"
#include <sstream>

USING_NS_CC;

namespace playcard {

LevelConfig LevelConfigLoader::loadLevelConfig(int levelId) const {
    LevelConfig out;
    std::string cfgPath;
    if (levelId <= 0) {
        cfgPath = "res/res/playfield.json";
    } else {
        std::ostringstream oss;
        oss << "res/res/level_" << levelId << ".json";
        cfgPath = oss.str();
        if (!FileUtils::getInstance()->isFileExist(cfgPath)) {
            cfgPath = "res/res/playfield.json";
        }
    }

    std::string raw;
    if (FileUtils::getInstance()->isFileExist(cfgPath)) {
        raw = FileUtils::getInstance()->getStringFromFile(cfgPath);
    }
    if (raw.empty()) {
        return out;
    }

    rapidjson::Document doc;
    doc.Parse(raw.c_str());
    if (doc.HasParseError() || !doc.IsObject()) {
        return out;
    }

    auto parsePosition = [](const rapidjson::Value& p) -> Position2D {
        Position2D pos;
        if (p.IsObject()) {
            pos.x = p.HasMember("x") ? p["x"].GetFloat() : 0.0f;
            pos.y = p.HasMember("y") ? p["y"].GetFloat() : 0.0f;
        }
        return pos;
    };

    if (doc.HasMember("Playfield") && doc["Playfield"].IsArray()) {
        const auto& arr = doc["Playfield"];
        for (rapidjson::SizeType i = 0; i < arr.Size(); ++i) {
            const auto& it = arr[i];
            CardPlacement c;
            c.cardFace = it.HasMember("CardFace") ? it["CardFace"].GetInt() : 0;
            c.cardSuit = it.HasMember("CardSuit") ? it["CardSuit"].GetInt() : 0;
            if (it.HasMember("Position")) {
                c.position = parsePosition(it["Position"]);
            }
            out.playfield.push_back(c);
        }
    }
    if (doc.HasMember("Stack") && doc["Stack"].IsArray()) {
        const auto& arr = doc["Stack"];
        for (rapidjson::SizeType i = 0; i < arr.Size(); ++i) {
            const auto& it = arr[i];
            CardPlacement c;
            c.cardFace = it.HasMember("CardFace") ? it["CardFace"].GetInt() : 0;
            c.cardSuit = it.HasMember("CardSuit") ? it["CardSuit"].GetInt() : 0;
            if (it.HasMember("Position")) {
                c.position = parsePosition(it["Position"]);
            }
            out.stack.push_back(c);
        }
    }
    return out;
}

} // namespace playcard
