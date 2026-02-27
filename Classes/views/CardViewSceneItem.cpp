#include "CardViewSceneItem.h"
#include "configs/models/CardResConfig.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

CardViewSceneItem* CardViewSceneItem::create(int face, int suit) {
	CardViewSceneItem* p = new (std::nothrow) CardViewSceneItem();
	if (p && p->init(face, suit)) {
		p->autorelease();
		return p;
	}
	delete p;
	return nullptr;
}

bool CardViewSceneItem::init(int face, int suit) {
	if (!cocos2d::ui::Button::init()) return false;
	_face = face;
	_suit = suit;
	_clicked = 0;

	// 1. 加载背景精灵（核心尺寸参考）
	std::string strbk = "res/res/card_general.png";
	_background = cocos2d::Sprite::create(strbk);
	if (!_background) { // 增加空指针保护
		return false;
	}
	_background->setAnchorPoint(Vec2(0.5f, 0.5f));
	_background->setPosition(Vec2::ZERO); // 相对于Button中心
	this->addChild(_background);

	// 2. 关键设置：关闭自动适配 + 手动设置Button尺寸（保证点击区域）
	this->ignoreContentAdaptWithSize(false);
	Size bgSize = _background->getContentSize();
	this->setContentSize(bgSize); // Button尺寸与背景一致
	this->setTouchEnabled(true);
	this->setEnabled(true);

	// 3. 加载主点数精灵（居中）
	std::string numPath = getFacePath(face);
	if (cocos2d::FileUtils::getInstance()->isFileExist(numPath)) {
		_numSp = cocos2d::Sprite::create(numPath);
	}
	if (_numSp) {
		_numSp->setAnchorPoint(Vec2(0.5f, 0.5f));
		_numSp->setPosition(Vec2::ZERO); // 居中
		this->addChild(_numSp);
	}
	else {
		auto placeholder = cocos2d::LayerColor::create(cocos2d::Color4B(200, 200, 200, 255), 60, 90);
		placeholder->setAnchorPoint(Vec2(0.5f, 0.5f));
		placeholder->setPosition(Vec2::ZERO);
		this->addChild(placeholder);
	}

	// 4. 加载左上角小点数精灵（统一坐标计算逻辑）
	std::string numSmallPath = getSmallFacePath(face);
	if (cocos2d::FileUtils::getInstance()->isFileExist(numSmallPath)) {
		_numSpSmall = cocos2d::Sprite::create(numSmallPath);
	}
	if (_numSpSmall) {
		_numSpSmall->setAnchorPoint(Vec2(0.0f, 1.0f)); // 左上角锚点
		const float marginLeft = 8.0f;
		const float marginTop = 8.0f;
		// 统一基于Button尺寸计算（Button锚点0.5,0.5，所以左上角坐标是 -bgSize.width/2, bgSize.height/2）
		float posX = -bgSize.width / 2.0f + marginLeft;
		float posY = bgSize.height / 2.0f - marginTop;
		_numSpSmall->setPosition(Vec2(posX, posY));
		this->addChild(_numSpSmall);
	}

	// 5. 加载右上角花色精灵（统一坐标计算逻辑）
	std::string suitPath = getSuitPath(suit);
	if (cocos2d::FileUtils::getInstance()->isFileExist(suitPath)) {
		_suitSp = cocos2d::Sprite::create(suitPath);
	}
	if (_suitSp) {
		_suitSp->setAnchorPoint(Vec2(1.0f, 1.0f)); // 右上角锚点
		const float marginRight = 8.0f;
		const float marginTop = 8.0f;
		// 统一基于Button尺寸计算
		float posX = bgSize.width / 2.0f - marginRight;
		float posY = bgSize.height / 2.0f - marginTop;
		_suitSp->setPosition(Vec2(posX, posY));
		this->addChild(_suitSp);
	}

	// 点击事件
	this->addClickEventListener([this](Ref* sender) {
		_clicked = !_clicked; // 简化逻辑
		setItemClicked(_clicked);
		});

	return true;
}

void CardViewSceneItem::updateTouchSize(const Vec2& pos)
{
	// 确保 Button 有合适的内容尺寸，否则点击区域可能为零
	//this->ignoreContentAdaptWithSize(false);
	//Size bgSize = _background->getContentSize();
	////Size thisSize = this->getContentSize();
	//this->setContentSize(bgSize);
	//this->setTouchEnabled(true);
	//this->setEnabled(true);

	_background->setPosition(Vec2::ZERO); // 相对于Button中心
}

void CardViewSceneItem::setItemClicked(int isClicked)
{
	if (isClicked)
	{
		std::string tempPath = getRedFacePath(_face);
		_numSp->setSpriteFrame(tempPath);
		tempPath = getRedSmallFacePath(_face);
		_numSpSmall->setSpriteFrame(tempPath);
		tempPath = getRedSuitPath(_face);
		_suitSp->setSpriteFrame(tempPath);
	}
	else
	{
		std::string tempPath = getFacePath(_face);
		_numSp->setSpriteFrame(tempPath);
		tempPath = getSmallFacePath(_face);
		_numSpSmall->setSpriteFrame(tempPath);
		tempPath = getSuitPath(_face);
		_suitSp->setSpriteFrame(tempPath);
	}
}

std::string CardViewSceneItem::getFacePath(int face)
{
	switch (face)
	{
	case CardFaceType::CFT_ACE:
		return ("res/res/number/big_black_A.png");
	case CardFaceType::CFT_TWO:
		return ("res/res/number/big_black_2.png");
	case CardFaceType::CFT_THREE:
		return ("res/res/number/big_black_3.png");
	case CardFaceType::CFT_FOUR:
		return ("res/res/number/big_black_4.png");
	case CardFaceType::CFT_FIVE:
		return ("res/res/number/big_black_5.png");
	case CardFaceType::CFT_SIX:
		return ("res/res/number/big_black_6.png");
	case CardFaceType::CFT_SEVEN:
		return ("res/res/number/big_black_7.png");
	case CardFaceType::CFT_EIGHT:
		return ("res/res/number/big_black_8.png");
	case CardFaceType::CFT_NINE:
		return ("res/res/number/big_black_9.png");
	case CardFaceType::CFT_TEN:
		return ("res/res/number/big_black_10.png");
	case CardFaceType::CFT_JACK:
		return ("res/res/number/big_black_J.png");
	case CardFaceType::CFT_QUEEN:
		return ("res/res/number/big_black_Q.png");
	case CardFaceType::CFT_KING:
		return ("res/res/number/big_black_K.png");
	default:
		break;
	}
	return ("res/res/number/big_black_A.png");
}

std::string CardViewSceneItem::getSmallFacePath(int face)
{
	switch (face)
	{
	case CardFaceType::CFT_ACE:
		return ("res/res/number/small_black_A.png");
	case CardFaceType::CFT_TWO:
		return ("res/res/number/small_black_2.png");
	case CardFaceType::CFT_THREE:
		return ("res/res/number/small_black_3.png");
	case CardFaceType::CFT_FOUR:
		return ("res/res/number/small_black_4.png");
	case CardFaceType::CFT_FIVE:
		return ("res/res/number/small_black_5.png");
	case CardFaceType::CFT_SIX:
		return ("res/res/number/small_black_6.png");
	case CardFaceType::CFT_SEVEN:
		return ("res/res/number/small_black_7.png");
	case CardFaceType::CFT_EIGHT:
		return ("res/res/number/small_black_8.png");
	case CardFaceType::CFT_NINE:
		return ("res/res/number/small_black_9.png");
	case CardFaceType::CFT_TEN:
		return ("res/res/number/small_black_10.png");
	case CardFaceType::CFT_JACK:
		return ("res/res/number/small_black_J.png");
	case CardFaceType::CFT_QUEEN:
		return ("res/res/number/small_black_Q.png");
	case CardFaceType::CFT_KING:
		return ("res/res/number/small_black_K.png");
	default:
		break;
	}
	return ("res/res/number/big_black_A.png");
}

std::string CardViewSceneItem::getSuitPath(int suit)
{
	switch (suit)
	{
	case CardSuitType::CST_CLUBS:
		return ("res/res/suits/club.png");
	case CardSuitType::CST_DIAMONDS:
		return ("res/res/suits/diamond.png");
	case CardSuitType::CST_HEARTS:
		return ("res/res/suits/heart.png");
	case CardSuitType::CST_SPADES:
		return ("res/res/suits/spade.png");
	default:
		break;
	}
}

std::string CardViewSceneItem::getRedFacePath(int face)
{
	switch (face)
	{
	case CardFaceType::CFT_ACE:
		return ("res/res/number/big_red_A.png");
	case CardFaceType::CFT_TWO:
		return ("res/res/number/big_red_2.png");
	case CardFaceType::CFT_THREE:
		return ("res/res/number/big_red_3.png");
	case CardFaceType::CFT_FOUR:
		return ("res/res/number/big_red_4.png");
	case CardFaceType::CFT_FIVE:
		return ("res/res/number/big_red_5.png");
	case CardFaceType::CFT_SIX:
		return ("res/res/number/big_red_6.png");
	case CardFaceType::CFT_SEVEN:
		return ("res/res/number/big_red_7.png");
	case CardFaceType::CFT_EIGHT:
		return ("res/res/number/big_red_8.png");
	case CardFaceType::CFT_NINE:
		return ("res/res/number/big_red_9.png");
	case CardFaceType::CFT_TEN:
		return ("res/res/number/big_red_10.png");
	case CardFaceType::CFT_JACK:
		return ("res/res/number/big_red_J.png");
	case CardFaceType::CFT_QUEEN:
		return ("res/res/number/big_red_Q.png");
	case CardFaceType::CFT_KING:
		return ("res/res/number/big_red_K.png");
	default:
		break;
	}
	return ("res/res/number/big_red_A.png");
}

std::string CardViewSceneItem::getRedSmallFacePath(int face)
{
	switch (face)
	{
	case CardFaceType::CFT_ACE:
		return ("res/res/number/small_red_A.png");
	case CardFaceType::CFT_TWO:
		return ("res/res/number/small_red_2.png");
	case CardFaceType::CFT_THREE:
		return ("res/res/number/small_red_3.png");
	case CardFaceType::CFT_FOUR:
		return ("res/res/number/small_red_4.png");
	case CardFaceType::CFT_FIVE:
		return ("res/res/number/small_red_5.png");
	case CardFaceType::CFT_SIX:
		return ("res/res/number/small_red_6.png");
	case CardFaceType::CFT_SEVEN:
		return ("res/res/number/small_red_7.png");
	case CardFaceType::CFT_EIGHT:
		return ("res/res/number/small_red_8.png");
	case CardFaceType::CFT_NINE:
		return ("res/res/number/small_red_9.png");
	case CardFaceType::CFT_TEN:
		return ("res/res/number/small_red_10.png");
	case CardFaceType::CFT_JACK:
		return ("res/res/number/small_red_J.png");
	case CardFaceType::CFT_QUEEN:
		return ("res/res/number/small_red_Q.png");
	case CardFaceType::CFT_KING:
		return ("res/res/number/small_red_K.png");
	default:
		break;
	}
	return ("res/res/number/big_red_A.png");
}

std::string CardViewSceneItem::getRedSuitPath(int suit)
{
	switch (suit)
	{
	case CardSuitType::CST_CLUBS:
		return ("res/res/suits/club_red.png");
	case CardSuitType::CST_DIAMONDS:
		return ("res/res/suits/diamond_red.png");
	case CardSuitType::CST_HEARTS:
		return ("res/res/suits/heart_red.png");
	case CardSuitType::CST_SPADES:
		return ("res/res/suits/spade_red.png");
	default:
		break;
	}
}


