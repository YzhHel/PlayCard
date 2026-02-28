#pragma once

#include "cocos2d.h"


class CardViewSceneItem : public cocos2d::Node {
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
	void changeSpritePicture(cocos2d::Sprite* sprite, std::string path);

	// 替代 Button 的 addClickEventListener：设置点击回调
	void setClickCallback(const std::function<void()>& callback);

private:
	int _face;
	int _suit;
	int _clicked;

	cocos2d::Sprite* _background = nullptr;	//背景
	cocos2d::Sprite* _numSp = nullptr;		//点数
	cocos2d::Sprite* _numSpSmall = nullptr;	//小点数
	cocos2d::Sprite* _suitSp = nullptr;		//花色

	// 新增：点击回调函数对象
	std::function<void()> _clickCallback;
};

