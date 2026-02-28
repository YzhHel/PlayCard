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

	// 读取当前牌面的点数与花色
	int getFace() const { return _face; }
	int getSuit() const { return _suit; }

	// 是否启用“仅顶牌可点”限制（默认开启）
	void setEnforceTopMost(bool enable) { _enforceTopMost = enable; }

	void setItemClicked(int isClicked = 0);
	void changeSpritePicture(cocos2d::ui::ImageView* image, std::string path);

	// 封装 Button 的 addClickEventListener：设置点击回调
	void setClickCallback(const std::function<void()>& callback);
	// 设置是否显示卡面
	void setIsShowUp(int isShowUp=0);
private:
	int _face;
	int _suit;
	int _clicked;
	int _isShowUp;

	cocos2d::ui::ImageView* _numSp = nullptr;		//点数
	cocos2d::ui::ImageView* _numSpSmall = nullptr;	//小点数
	cocos2d::ui::ImageView* _suitSp = nullptr;		//花色

	// 点击回调函数对象
	std::function<void()> _clickCallback;

	// 是否启用“顶牌限制”逻辑
	bool _enforceTopMost = true;
};

