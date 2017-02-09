#pragma once

#include "cocos2d.h"

class Select : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
	virtual void update(float delta);

    CREATE_FUNC(Select);

private:
	int mLevel;

	void PushStart(Ref* pSender);

	void changeLevelColor(cocos2d::Node*);
};
