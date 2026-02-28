#pragma once

#include "configs/models/LevelConfig.h"
#include <string>

namespace playcard {

/** 根据关卡 ID 加载关卡配置（如从 res/res/level_<id>.json 或 playfield.json） */
class LevelConfigLoader {
public:
    /** 加载指定关卡的配置，失败返回空（或可改为 optional/bool+out）。路径规则：levelId<=0 用 playfield.json，否则 level_<id>.json */
    LevelConfig loadLevelConfig(int levelId) const;
};

} // namespace playcard
