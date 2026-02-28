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
	_isShowUp = 1;

	// 1. 设置 Button 背景纹理（作为牌的底图）
	std::string strbk = "res/res/card_general.png";
	this->loadTextureNormal(strbk);
	// 让 Button 大小跟随纹理尺寸，保证点击区域与牌面一致
	this->ignoreContentAdaptWithSize(true);
	Size bgSize = this->getContentSize();

	// 3. 加载主点数图片（居中，作为 Button 内容的一部分）
	std::string numPath = getFacePath(face);
	if (cocos2d::FileUtils::getInstance()->isFileExist(numPath)) {
		_numSp = cocos2d::ui::ImageView::create(numPath);
	}
	if (_numSp) {
		_numSp->setAnchorPoint(Vec2(0.5f, 0.5f));
		// Button 的子节点坐标原点在左下角，这里用一半宽高居中
		_numSp->setPosition(Vec2(bgSize.width / 2.0f, bgSize.height / 2.0f));
		this->addChild(_numSp);
	}
	else {
		auto placeholder = cocos2d::LayerColor::create(cocos2d::Color4B(200, 200, 200, 255), 60, 90);
		placeholder->setAnchorPoint(Vec2(0.5f, 0.5f));
		placeholder->setPosition(Vec2::ZERO);
		this->addChild(placeholder);
	}

	// 4. 加载左上角小点数图片（统一坐标计算逻辑）
	std::string numSmallPath = getSmallFacePath(face);
	if (cocos2d::FileUtils::getInstance()->isFileExist(numSmallPath)) {
		_numSpSmall = cocos2d::ui::ImageView::create(numSmallPath);
	}
	if (_numSpSmall) {
		_numSpSmall->setAnchorPoint(Vec2(0.0f, 1.0f)); // 左上角锚点
		const float marginLeft = 8.0f;
		const float marginTop = 8.0f;
		// Button 子节点坐标系左下为 (0,0)，右上为 (bgSize.width, bgSize.height)
		float posX = marginLeft;
		float posY = bgSize.height - marginTop;
		_numSpSmall->setPosition(Vec2(posX, posY));
		this->addChild(_numSpSmall);
	}

	// 5. 加载右上角花色图片（统一坐标计算逻辑）
	std::string suitPath = getSuitPath(suit);
	if (cocos2d::FileUtils::getInstance()->isFileExist(suitPath)) {
		_suitSp = cocos2d::ui::ImageView::create(suitPath);
	}
	if (_suitSp) {
		_suitSp->setAnchorPoint(Vec2(1.0f, 1.0f)); // 右上角锚点
		const float marginRight = 8.0f;
		const float marginTop = 8.0f;
		// 右上角：宽度减去右边距，高度减去上边距
		float posX = bgSize.width - marginRight;
		float posY = bgSize.height - marginTop;
		_suitSp->setPosition(Vec2(posX, posY));
		this->addChild(_suitSp);
	}

	// 6. 使用 Button 自带的点击事件（addClickEventListener）
	this->addClickEventListener([this](cocos2d::Ref* sender) {
		if (_clickCallback)
		{
			_clicked = (_clicked == 0) ? 1 : 0;
			this->setItemClicked(_clicked);
			_clickCallback();
		}
	});

	return true;
}

// 替代 Button 的 addClickEventListener：设置外部回调
void CardViewSceneItem::setClickCallback(const std::function<void()>& callback)
{
	_clickCallback = callback;
}

void CardViewSceneItem::setItemClicked(int isClicked)
{
	if (_isShowUp == 0)
	{
		changeSpritePicture(_numSp, "");

		changeSpritePicture(_numSpSmall, "");

		changeSpritePicture(_suitSp, "");
		return;
	}

	if (isClicked)
	{
		changeSpritePicture(_numSp, getRedFacePath(_face));

		changeSpritePicture(_numSpSmall, getRedSmallFacePath(_face));

		changeSpritePicture(_suitSp, getRedSuitPath(_suit));
	}
	else
	{
		changeSpritePicture(_numSp, getFacePath(_face));

		changeSpritePicture(_numSpSmall, getSmallFacePath(_face));

		changeSpritePicture(_suitSp, getSuitPath(_suit));
	}
}

void CardViewSceneItem::setIsShowUp(int isShowUp)
{
	if (isShowUp == _isShowUp)
		return;
	_isShowUp = isShowUp;
	setItemClicked(_clicked);
}

void CardViewSceneItem::changeSpritePicture(cocos2d::ui::ImageView* image, std::string path)
{
	if (!image)
		return;

	// 空路径：视为隐藏该图片
	if (path.empty())
	{
		image->setVisible(false);
		return;
	}

	image->loadTexture(path);
	image->setVisible(true);
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


