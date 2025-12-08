#pragma once

#include <cpp-reactive.hpp>
#include <cocos2d.h>
#include <Geode/utils/cocos.hpp>
#include <Geode/UI.hpp>
#include <UIBuilder.hpp>

namespace camila {
	using namespace cppreactive;

	inline int initialize_observer = []() {
		Build<cocos2d::CCScheduler>::schedule([](float) {
			ObserverStack::shared()->update();
		});

		return 0;
	}();
}