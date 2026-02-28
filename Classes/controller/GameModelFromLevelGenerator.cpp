#include "GameModelFromLevelGenerator.h"

namespace playcard {

GameModel GameModelFromLevelGenerator::generateGameModel(int levelId, const LevelConfig& levelConfig) const {
    GameModel model;
    model.levelId = levelId;
    model.playfield = levelConfig.playfield;
    model.stack = levelConfig.stack;
    return model;
}

} // namespace playcard
