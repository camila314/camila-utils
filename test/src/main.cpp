#include <Geode/Geode.hpp>
using namespace geode::prelude;

#include <camila/utils.hpp>
#include <UIBuilder.hpp>


#include <Geode/utils/coro.hpp>

using namespace geode::utils::coro;
CoTask<void> nextFrame() {
	auto [task, post, prog, cancel] = Task<void>::spawn("<Next Frame>");

	queueInMainThread([post]() {
		post(true);
	});

	return std::move(task);
}

CoTask<void> sleep(double seconds) {
	return Task<void>::run([seconds](auto, auto) {
		std::this_thread::sleep_for(std::chrono::duration<double>(seconds));
		return true;
	}, "<Sleep>");
}

CoTask<void> onButton(CCMenuItemSprite* ext) {
	auto [task, post, prog, cancel] = Task<void>::spawn("<Button Callback>");
	auto cb = uibuilder::BuildCallback<CCMenuItemSpriteExtra>::create([post](auto) {
		post(true);
	});
	ext->addChild(cb);
	ext->setTarget(cb, menu_selector(uibuilder::BuildCallback<CCMenuItemSpriteExtra>::onCallback));

	return std::move(task);
}


int main() {
	if (camila::Setting<"does-do-thing">()) {
		// do thing
	}

	camila::Setting<"does-do-thing">().react([](bool setting) {
		log::info("Changed!! {}", setting);
	});
}