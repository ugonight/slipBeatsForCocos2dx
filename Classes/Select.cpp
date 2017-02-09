#include "Select.h"
#pragma execution_character_set("utf-8")

#include "SimpleAudioEngine.h"

using namespace CocosDenshion;
USING_NS_CC;

Scene* Select::createScene()
{
    auto scene = Scene::create();
    auto layer = Select::create();
    scene->addChild(layer);
    return scene;
}

bool Select::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
	this->scheduleUpdate();

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto listener = EventListenerTouchOneByOne::create();

	//背景
	{
		auto back = Sprite::create("select_back.png");
		back->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
		addChild(back, 0, "back0");

		BlendFunc blend;
		blend.src = GL_SRC_ALPHA;
		blend.dst = GL_ONE;

		back = Sprite::create("select_back_p.png");
		back->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
		back->setBlendFunc(blend);
		back->setOpacity(0);
		back->runAction(RepeatForever::create(Sequence::create(
			EaseSineInOut::create(FadeIn::create(3.0f)),
			EaseSineInOut::create(FadeOut::create(3.0f)),
			NULL)));
		addChild(back, 1, "back1");
	}

	//矢印
	{
		auto left = Sprite::create("left.png");
		left->setPosition(Vec2(40 + origin.x, visibleSize.height * 3 / 5 + 32 + origin.y));
		left->setOpacity(0);
		addChild(left, 2, "left");
		auto right = Sprite::create("right.png");
		right->setPosition(Vec2(visibleSize.width - 40 + origin.x, visibleSize.height * 3 / 5 + 32 + origin.y));
		right->setOpacity(0);
		addChild(right, 2, "right");

		auto fade = RepeatForever::create(Sequence::create(
			EaseSineInOut::create(FadeIn::create(1.0f)), 
			EaseSineInOut::create(FadeOut::create(1.0f)),
			NULL)
		);
		left->runAction(fade);
		right->runAction(fade->clone());
	}

	//ステータス
	{
		auto status = Sprite::create("status.png");
		status->setPosition(visibleSize / 2);
		status->setOpacity(180);
		addChild(status, 2, "status");

		//レベル
		auto button = Sprite::create("button2.png");
		button->setPosition(Vec2(65 + 45 + origin.x, visibleSize.height - 48 - 360 + origin.y));
		changeLevelColor(button);
		addChild(button, 3, "levelButton");
		listener->onTouchBegan = [this](Touch* touch, Event* event) {
			if (event->getCurrentTarget()->getBoundingBox().containsPoint(touch->getLocation())) {
				mLevel++;
				mLevel %= 4;
				UserDefault::getInstance()->setIntegerForKey("level",mLevel);
				UserDefault::getInstance()->flush();

				auto button = getChildByName("levelButton");
				changeLevelColor(button);

				return true;
			}
			return false;
		};
		this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, button);

		//決定
		auto ok = MenuItemImage::create(
			"button3_off.png",
			"button3_on.png",
			CC_CALLBACK_1(Select::PushStart, this)
		);

		ok->setPosition(Vec2(origin.x + 610 + ok->getContentSize().width / 2,
			origin.y + visibleSize.height - 360 - ok->getContentSize().height / 2));

		// create menu, it's an autorelease object
		auto menu = Menu::create(ok, NULL);
		menu->setPosition(Vec2::ZERO);
		this->addChild(menu, 1);

	}

	//変数
	{
		mLevel = UserDefault::getInstance()->getIntegerForKey("level");

		SimpleAudioEngine::sharedEngine()->preloadEffect("ok.ogg");
	}
 
    return true;
}

void Select::update( float delta ) {

}

void Select::PushStart(Ref* pSender) {
	SimpleAudioEngine::sharedEngine()->playEffect("ok.ogg");
}

void Select::changeLevelColor(cocos2d::Node* node) {
	switch (mLevel)
	{
	case 0:
		node->runAction(TintTo::create(1, Color3B::GREEN));
		break;
	case 1:
		node->runAction(TintTo::create(1, Color3B::BLUE));
		break;
	case 2:
		node->runAction(TintTo::create(1, Color3B::YELLOW));
		break;
	case 3:
		node->runAction(TintTo::create(1, Color3B::RED));
		break;
	default:
		break;
	}
}