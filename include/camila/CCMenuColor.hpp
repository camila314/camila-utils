#pragma once

#include <cocos2d.h>

namespace camila {
	class CCMenuColor : public cocos2d::CCMenu {
	 public:
	    inline static CCMenuColor* create() {
	        auto ret = new CCMenuColor();
	        if (ret->init()) {
	            ret->setPosition({0, 0});
	            ret->setAnchorPoint({0, 0});
	            ret->autorelease();
	            return ret;
	        }
	        delete ret;
	        return nullptr;
	    }

	    inline void draw() override {
	        auto c = cocos2d::ccc4FFromccc3B(getColor());
	        c.a = getOpacity() / 255.0f;
	        cocos2d::ccDrawSolidRect({0, 0}, getContentSize(), c);
	    }
	};
}