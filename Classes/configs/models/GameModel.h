#pragma once

#include "LevelConfig.h"
#include <memory>

namespace playcard {

/** 运行时游戏数据模型，由 LevelConfig 生成，供各控制器使用 */
struct GameModel {
    int levelId = 0;
    std::vector<CardPlacement> playfield;
    std::vector<CardPlacement> stack;
    // 可扩展：得分、步数、状态等
};

} // namespace playcard
