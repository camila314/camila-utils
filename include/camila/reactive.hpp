#pragma once

#include <cpp-reactive.hpp>
#include <cocos2d.h>
#include <Geode/utils/cocos.hpp>
#include <Geode/UI.hpp>
#include <UIBuilder.hpp>

namespace camila {
	using namespace cppreactive;
	using namespace geode::prelude;

	inline int initialize_observer = []() {
		Build<cocos2d::CCScheduler>::schedule([](float) {
			ObserverStack::shared()->update();
		});

		return 0;
	}();

	template <typename F>
	auto bindReact(CCNode* obj, F&& func) {
		Observatory* ob;

		if (auto wrapper = typeinfo_cast<ObjWrapper<Observatory>*>(obj->getUserObject("cppreactive-observer"))) {
			ob = &wrapper->getValue();
		} else {
			auto wrap = ObjWrapper<Observatory>::create(std::move(Observatory{}));
			ob = &wrap->getValue();
			obj->setUserObject("cppreactive-observer", wrap);
		}

		return ob->reactToChanges(std::forward<F>(func));
	}
}