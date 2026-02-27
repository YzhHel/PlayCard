#ifndef GameViewScene_H
#define GameViewScene_H

#include "cocos2d.h"
#include "CardViewSceneItem.h"


class GameViewScene : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();
    virtual bool init() override;
    CREATE_FUNC(GameViewScene);
};

#endif // GameViewScene_H
