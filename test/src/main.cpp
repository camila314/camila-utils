#include <Geode/Geode.hpp>
using namespace geode::prelude;

#include <camila/utils.hpp>


int main() {
	if (camila::Setting<"does-do-thing">()) {
		// do thing
	}

	camila::Setting<"does-do-thing">().react([](bool setting) {
		log::info("Changed!! {}", setting);
	});
}