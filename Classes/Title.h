#pragma once

#include "cocos2d.h"

class Title : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
	virtual void update(float delta);

    CREATE_FUNC(Title);

private:
	int mTime, mTime2;

	void HSVtoRGB(int h, int s, int v, int* rgb);
};
