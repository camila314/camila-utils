#pragma once

#include <Geode/loader/Mod.hpp>
#include <Geode/utils/string.hpp>
#include <Geode/utils/coro.hpp>
#include <Geode/Result.hpp>
#include <UIBuilder.hpp>
#include <cpp-reactive.hpp>
#include <Geode/ui/ScrollLayer.hpp>

namespace camila {
	using namespace geode::prelude;
	using namespace geode::utils::string;
	using namespace cppreactive;

	inline int initialize_observer = []() {
		Build<cocos2d::CCScheduler>::schedule([](float) {
			cppreactive::ObserverStack::shared()->update();
		});

		return 0;
	}();

	template <ConstexprString S, typename T = bool>
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

	class CCMenuColor : public CCMenu {
	 public:
	    inline static CCMenuColor* create() {
	        auto ret = new CCMenuColor();
	        if (ret->init()) {
	            ret->setPosition({0, 0});
	            ret->setAnchorPoint({0, 0});
	            ret->autorelease();
	            return ret;
	        }
	        delete ret;
	        return nullptr;
	    }

	    inline void draw() override {
	        auto c = ccc4FFromccc3B(getColor());
	        c.a = getOpacity() / 255.0f;
	        cocos2d::ccDrawSolidRect({0, 0}, getContentSize(), c);
	    }
	};

	template <typename Data>
	class ScrollableListView : public ScrollLayer {
	    Signal<std::vector<Data>> m_data;
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
};
