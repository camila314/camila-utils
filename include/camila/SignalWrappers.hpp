#pragma once

#include "reactive.hpp"
#include <functional>
#include <cocos2d.h>

namespace camila {
	inline std::function<void()> ButtonSignal(Signal<std::monostate>& sig) {
		auto ref = sig.ref();
		return [ref]() mutable {
			*ref = {};
		};
	}

	inline std::function<void(CCMenuItemToggler*)> ToggleSignal(Signal<bool>& sig) {
		auto ref = sig.ref();
		return [ref](CCMenuItemToggler* tog) mutable {
			*ref = tog->isToggled();
		};
	}
}