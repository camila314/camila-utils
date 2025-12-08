#pragma once
#include <Geode/utils/cocos.hpp>

namespace camila {
	using namespace geode::prelude;

	inline void tetherObject(CCNode* parent, CCObject* child) {
		auto arr = typeinfo_cast<CCArray*>(parent->getUserObject("camila-objs"));

		if (!arr) {
			arr = CCArray::create();
			parent->setUserObject("camila-objs", arr);
		}

		arr->addObject(child);
	}

	template <typename T>
	T& tetherData(CCNode* parent, T&& data) {
		auto obj = ObjWrapper<std::remove_reference_t<T>>::create(std::forward<T>(data));
		tetherObject(parent, obj);
		return obj->getValue();
	}
}