#include <Geode/loader/Mod.hpp>
#include <Geode/utils/string.hpp>
#include <Geode/Result.hpp>
#include <cpp-reactive.hpp>

namespace camila {
	using namespace geode::prelude;
	using namespace geode::utils::string;
	using namespace cppreactive;

	template <ConstexprString S, typename T = bool>
	Reactive<T>& Setting() {
		static Reactive<T> setting = Mod::get()->getSettingValue<T>(S.data());
		static EventListener<SettingChangedFilterV3> listener({Mod::get(), std::string(S.data())});

		static bool bind = (
			listener.bind([&](auto) {
				setting = Mod::get()->getSettingValue<T>(S.data());
			}),
			true
		);

		return setting;
	}
};
