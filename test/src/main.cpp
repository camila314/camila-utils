#include "Geode/utils/cocos.hpp"
#include "ccTypes.h"
#include <Geode/Geode.hpp>

#include <camila/Selector.hpp>
#include <camila/ObjectData.hpp>
#include <camila/Dialog.hpp>
#include <Geode/utils/coro.hpp>

using namespace geode::prelude;
using namespace camila;

#include <Geode/modify/MenuLayer.hpp>

class $modify(MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;

        /*auto& mydouble = camila::tetherData<Signal<double>>(this, 35.0);

        Build<camila::Slider>::create(mydouble.ref())
            .parent(this)
            .center();

        auto label = Build<CCLabelBMFont>::create("", "bigFont.fnt")
            .parent(this)
            .center().move(0, 40)
            .collect();
        camila::bindReact(label, [&, label] {
            label->setString(std::to_string(*mydouble).c_str());
        });

        Build<ButtonSprite>::create("Increment")
            .intoMenuItem([&] {
                $async(&) {
                    if (co_await camila::Confirmation::create("Do you really wanna do this?")->showAndWait())
                        *mydouble = *mydouble + 1.0;
                };
            })
            .intoNewParent(CCMenu::create())
            .parent(this)
            .center().move(0, -40);*/


        auto& sig = camila::tetherData<Signal<int>>(this, 0);
        std::vector<std::string> choices = {
            "thing 1",
            "thing 2",
            "thing 3"
        };
        Build<Selector>::create(sig.ref(), choices)
            .parent(this)
            .center();


        /*auto lv = camila::ScrollableListView<std::string>::create([](camila::CCMenuColor* cell, std::string const& text, int idx) {
            Build(cell)
                .color(idx % 2 == 0 ? ccc3(150, 80, 40) : ccc3(40, 80, 150))
                .height(30)
                .intoNewChild(CCLabelBMFont::create(text.c_str(), "bigFont.fnt"))
                    .center();
        }, {"hello","hi", "data!", "scroll list view", "crazy"});

        lv->setAnchorPoint({0.5, 0.5});
        lv->setContentSize({ 300, 200 });
        lv->setPosition({ 284, 160 });
        this->addChild(lv);

        lv->data()[3] = "modified!";

        $async() {        
            auto confirm = Confirmation::create("Do you <cr>want</c> to?", {.noBtn = "No way!"});
            
            confirm->show();
            bool result = co_await confirm->task();

            log::info("Confirmation result: {}", result ? "Yes" : "No");
            co_return;
        };*/
        return true;
    }

};
