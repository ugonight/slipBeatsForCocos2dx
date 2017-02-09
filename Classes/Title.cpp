#include "Title.h"
#include "Select.h"
#pragma execution_character_set("utf-8")

#include "SimpleAudioEngine.h"

using namespace CocosDenshion;
USING_NS_CC;

Scene* Title::createScene()
{
    auto scene = Scene::create();
    auto layer = Title::create();
    scene->addChild(layer);
    return scene;
}

bool Title::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
	this->scheduleUpdate();

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
 
	//背景
	{
		auto back = Sprite::create("title_back.png");
		back->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
		addChild(back, 0, "back0");

		BlendFunc blend;
		blend.src = GL_SRC_ALPHA;
		blend.dst = GL_ONE;

		back = Sprite::create("title_back_r.png");
		back->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
		back->setBlendFunc(blend);
		addChild(back, 1, "back1");
		back = Sprite::create("title_back_b.png");
		back->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
		back->setBlendFunc(blend);
		addChild(back, 1, "back2");
	}

	//ロゴ
	{
		auto logo = Sprite::create("title.png");
		logo->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
		addChild(logo, 2, "logo");
	}

	//スタート
	{
		auto label = Label::createWithTTF("CLICK TO START", "fonts/misaki_gothic.ttf", 40);
		label->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 4 + origin.y));
		label->enableShadow();
		label->setOpacity(0.0f);
		label->runAction(RepeatForever::create(Sequence::create(EaseSineInOut::create(FadeIn::create(1.0f)), EaseSineInOut::create(FadeOut::create(1.0f)),NULL)));
		addChild(label, 2, "label");
	}

	//タッチ
	{
		auto listener = EventListenerTouchOneByOne::create();
		listener->onTouchBegan = [this](Touch* touch, Event* event) {
			getChildByName("label")->stopAllActions();
			getChildByName("label")->runAction(RepeatForever::create(Sequence::create(FadeIn::create(0.1f), FadeOut::create(0.1f), NULL)));
			SimpleAudioEngine::sharedEngine()->playEffect("ok.ogg");

			//シーンチェンジ
			this->runAction(Sequence::create(DelayTime::create(1), CallFunc::create([this]() {
				auto select = Select::createScene();
				Director::getInstance()->replaceScene(TransitionFadeTR::create(1.0f, select));
			}), NULL));

			return true;
		};
		this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
	}

	SimpleAudioEngine::sharedEngine()->preloadEffect("ok.ogg");

	mTime = mTime2 = 0;

    return true;
}

void Title::update( float delta ) 
{
	mTime2++;
	mTime2 %= 32767;

	int rgb[3];
	HSVtoRGB(abs((int)(sin(mTime2 / 500.0) * 360.0)), 255, 255, rgb);
	getChildByName("back0")->setColor(Color3B(rgb[0], rgb[1], rgb[2]));
	getChildByName("back1")->setOpacity((int)(sin(mTime2 / 30.0) * 80.0 + 125.0));
	getChildByName("back2")->setOpacity((int)(sin(mTime2 / 30.0 + 3.14) * 80.0 + 125.0));
}

//HSV→RGB変換
//参考:http://www.technotype.net/tutorial/tutorial.php?fileId=%7BImage%20processing%7D&sectionId=%7B-converting-between-rgb-and-hsv-color-space%7D
void Title::HSVtoRGB(int h, int s, int v, int* rgb) {
	float f;
	int i, p, q, t;
	//int* rgb = new int[3];

	i = (int)floor(h / 60.0f) % 6;
	f = (float)(h / 60.0f) - (float)floor(h / 60.0f);
	p = (int)round(v * (1.0f - (s / 255.0f)));
	q = (int)round(v * (1.0f - (s / 255.0f) * f));
	t = (int)round(v * (1.0f - (s / 255.0f) * (1.0f - f)));

	switch (i) {
	case 0: rgb[0] = v; rgb[1] = t; rgb[2] = p; break;
	case 1: rgb[0] = q; rgb[1] = v; rgb[2] = p; break;
	case 2: rgb[0] = p; rgb[1] = v; rgb[2] = t; break;
	case 3: rgb[0] = p; rgb[1] = q; rgb[2] = v; break;
	case 4: rgb[0] = t; rgb[1] = p; rgb[2] = v; break;
	case 5: rgb[0] = v; rgb[1] = p; rgb[2] = q; break;
	}

	//return rgb;
}