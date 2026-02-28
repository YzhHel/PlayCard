#pragma once

#include "configs/models/LevelConfig.h"
#include "configs/models/GameModel.h"

namespace playcard {

/** 从 LevelConfig 生成运行时 GameModel */
class GameModelFromLevelGenerator {
public:
    /** 根据关卡配置生成游戏数据模型，levelId 由调用方传入（用于 startGame(levelId)） */
    GameModel generateGameModel(int levelId, const LevelConfig& levelConfig) const;
};

} // namespace playcard
