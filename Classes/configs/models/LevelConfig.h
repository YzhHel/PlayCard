#pragma once

#include <vector>

namespace playcard {

struct Position2D {
    float x = 0.0f;
    float y = 0.0f;
};

struct CardPlacement {
    int cardFace = 0;
    int cardSuit = 0;
    Position2D position;
};

/** 关卡配置：从 JSON 加载的静态关卡数据 */
struct LevelConfig {
    std::vector<CardPlacement> playfield;
    std::vector<CardPlacement> stack;
};

} // namespace playcard
