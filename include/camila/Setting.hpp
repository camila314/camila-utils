#pragma once

#include <Geode/utils/string.hpp>
#include <Geode/loader/Mod.hpp>
#include "reactive.hpp"

namespace camila {
	using namespace geode::prelude;

	template <geode::utils::string::ConstexprString S, typename T = bool>
	Signal<T>& Setting() {
		static Signal<T> setting = Mod::get()->getSettingValue<T>(S.data());
		static EventListener<SettingChangedFilterV3> listener({Mod::get(), std::string(S.data())});

		static bool bind = (
			listener.bind([&](auto) {
				*setting = Mod::get()->getSettingValue<T>(S.data());
			}),
			true
		);

		return setting;
	}
}