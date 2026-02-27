#ifndef PLAYCARDMAINVIEWSCENE_H
#define PLAYCARDMAINVIEWSCENE_H

#include "cocos2d.h"

class PlayCardMainViewScene : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();
    virtual bool init() override;
    CREATE_FUNC(PlayCardMainViewScene);
};

#endif // PLAYCARDMAINVIEWSCENE_H
