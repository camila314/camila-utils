#include <Geode/Geode.hpp>

#include <camila/utils.hpp>
#include <UIBuilder.hpp>

#include <Geode/utils/coro.hpp>

using namespace geode::prelude;
using namespace camila;

template <typename Data>
class ScrollableListView : public ScrollLayer {
    cppreactive::Signal<std::vector<Data>> m_data;
    std::function<void(camila::CCMenuColor*, Data const&, int)> m_cellSetup;
    Observatory m_observatory;
    Signal<CCSize> m_contentSize;
 public:
    void build(float width) {
        m_contentLayer->removeAllChildren();

        int i = 0;
        for (auto const& d : *m_data) {
            auto cell = camila::CCMenuColor::create();
            cell->setContentWidth(width);
            m_cellSetup(cell, d, i++);
            m_contentLayer->addChild(cell);
        }
    }

    void update(float) override {
        float before = m_contentLayer->getContentHeight();
        m_contentLayer->updateLayout();
        float after = m_contentLayer->getContentHeight();

        if (before != after) {
            m_contentLayer->setPositionY(m_contentLayer->getPositionY() + before - after);
            scrollWheel(0, 0);
        }

        if (*m_contentSize != getContentSize()) {
            *m_contentSize = getContentSize();
        }
    }

    ScrollableListView(decltype(m_cellSetup)&& cellSetup, std::vector<Data>&& vec) : ScrollLayer(CCRectZero, true, true) {
        *m_data = std::move(vec);
        m_cellSetup = std::move(cellSetup);

        ignoreAnchorPointForPosition(false);
        scheduleUpdate();

        m_observatory.reactToChanges([&] {
            m_contentLayer->setLayout(ColumnLayout::create()
                ->setGap(0)
                ->setAutoGrowAxis(m_contentSize->get().height)
                ->setAxisAlignment(AxisAlignment::End)
                ->setAxisReverse(true));

            this->build(m_contentSize->get().width);
        });
    }

    auto& data() {
        return *m_data;
    }

    static ScrollableListView* create(decltype(m_cellSetup)&& cellSetup, std::vector<Data>&& vec = {}) {
        auto ret = new ScrollableListView(std::move(cellSetup), std::move(vec));
        ret->autorelease();
        return ret;
    }
};

#include <Geode/modify/MenuLayer.hpp>
class $modify(MenuLayer) {

    bool init() {
        if (!MenuLayer::init()) return false;

        auto lv = ScrollableListView<std::string>::create([](camila::CCMenuColor* cell, std::string const& text, int idx) {
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

        return true;
    }

};
