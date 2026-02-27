#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class CardViewSceneItem : public cocos2d::ui::Button {
public:
	static CardViewSceneItem* create(int face, int suit);
	bool init(int face, int suit);
	std::string getFacePath(int face);
	std::string getSmallFacePath(int face);
	std::string getSuitPath(int suit);
	std::string getRedFacePath(int face);
	std::string getRedSmallFacePath(int face);
	std::string getRedSuitPath(int suit);

	void setItemClicked(int isClicked = 0);

	void updateTouchSize(const cocos2d::Vec2& pos);
		
private:
	int _face;
	int _suit;
	int _clicked;

	cocos2d::Sprite* _background = nullptr;	//背景
	cocos2d::Sprite* _numSp = nullptr;		//点数
	cocos2d::Sprite* _numSpSmall = nullptr;	//小点数
	cocos2d::Sprite* _suitSp = nullptr;		//花色
};

