#pragma once

#include <vector>
#include <Geode/ui/ScrollLayer.hpp>
#include "reactive.hpp"
#include "CCMenuColor.hpp"

namespace camila {
	using namespace geode::prelude;

	template <typename Data>
	class ScrollableListView : public geode::ScrollLayer {
	    cppreactive::Signal<std::vector<Data>> m_data;
	    std::function<void(camila::CCMenuColor*, Data const&, int)> m_cellSetup;
	    cppreactive::Observatory m_observatory;
	    Signal<cocos2d::CCSize> m_contentSize;
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
}