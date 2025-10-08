#include <Geode/loader/Mod.hpp>
#include <Geode/utils/string.hpp>
#include <Geode/utils/coro.hpp>
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

	namespace coro {
		using namespace geode::utils::coro;
		inline CoTask<void> nextFrame() {
			auto [task, post, prog, cancel] = Task<void>::spawn("<Next Frame>");

			queueInMainThread([post]() {
				post(true);
			});

			return CoTask<void>(std::move(task));
		}

		inline CoTask<void> sleep(double seconds) {
			return Task<void>::run([seconds](auto, auto) {
				std::this_thread::sleep_for(std::chrono::duration<double>(seconds));
				return true;
			}, "<Sleep>");
		}

		inline CoTask<bool> asyncPopup(char const* title, std::string const& text, char const* noBtn, char const* yesBtn) {
			auto [task, post, prog, cancel] = Task<bool>::spawn("<Button Callback>");

			createQuickPopup(title, text, noBtn, yesBtn, [post](auto, bool ok) {
				post(ok);
			});

			return std::move(task);
		}
	}
};
