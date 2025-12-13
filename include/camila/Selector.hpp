#pragma once

#include <cocos2d.h>
#include <Geode/utils/cocos.hpp>
#include "reactive.hpp"
#include "CCMenuColor.hpp"

namespace camila {
	using namespace geode::prelude;
	class Selector : public CCLayerColor {
	    // initializable
	    RefSignal<int> m_signal;
	    std::vector<std::string> m_choices;
	    std::string m_font = std::string("bigFont.fnt");
	    float m_fontScale = 0.5;
	    ccColor3B m_selColor = ccc3(35, 30, 150);

	    // state
	    bool m_dirty = true;
	    CCNode m_labels;
	    std::vector<Ref<CCLabelBMFont>> m_labelCache;
	    CCSprite m_carat;
	    bool m_open = false;
	 public:
	    Selector(RefSignal<int> sig, std::vector<std::string> choices) : m_signal(std::move(sig)), m_choices(std::move(choices)) {
	        CCLayerColor::initWithColor(ccc4(148, 78, 39, 255));

	        m_carat.retain();
	        m_carat.initWithSpriteFrameName("edit_upBtn_001.png");
	        m_carat.setRotation(180);
	        m_carat.setScale(0.5);

	        m_labels.init();
	        m_labels.retain();
	        m_labels.setLayout(ColumnLayout::create()->setAutoGrowAxis(0)->setGap(0)->setAxisReverse(true));
	        m_labels.setAnchorPoint({0, 1});

	        Build(this)
	            .contentSize(100, 20)
	            .ignoreAnchorPointForPos(false)
	            .initTouch();

	        m_signal->react([&](int idx) {

	        });
	    }

	    static Selector* create(RefSignal<int> sig, std::vector<std::string> choices) {
	        return new Selector(std::move(sig), std::move(choices));
	    }
	    static Selector* create(RefSignal<int> sig, std::initializer_list<char const*> choices) {
	    	std::vector<std::string> vec;
	    	for (auto c : choices) {
	    		vec.emplace_back(c);
	    	}
	        return new Selector(std::move(sig), std::move(vec));
	    }

	    auto font(std::string font) {
	        m_font = std::move(font);
	        m_dirty = true;
	        return Build(this);
	    }

	    auto fontScale(float scale) {
	        m_fontScale = scale;
	        m_dirty = true;
	        return Build(this);
	    }

	    auto selectColor(ccColor3B color) {
	        m_selColor = color;
	        m_dirty = true;
	        return Build(this);
	    }

	    auto choices(std::vector<std::string> choices) {
	        m_choices = std::move(choices);
	        m_dirty = true;
	        return Build(this);
	    }

	    auto addChoice(std::string choice) {
	        m_choices.push_back(std::move(choice));
	        m_dirty = true;
	        return Build(this);
	    }

	    std::span<std::string const> getChoices() const {
	        return m_choices;
	    }

	    void draw() override {
	        CCLayerColor::draw();

	        if (m_dirty) {
	            m_labels.removeAllChildren();
	            m_labelCache.clear();

	            for (auto& choice : m_choices) {
	                Build<CCLabelBMFont>::create(choice.c_str(), m_font.c_str())
	                    .scale(m_fontScale)
	                    .anchorPoint({0, 0.5})
	                    .pos(5, getContentHeight() / 2)
	                    .store(m_labelCache.emplace_back())
	                    .intoNewParent(CCMenuColor::create())
	                        .cascadeColor(false)
	                        .color(getColor())
	                        .contentSize(getContentSize() - ccp(20, 0))
	                        .parent(&m_labels);
	            }
	            m_labels.updateLayout();
	            m_dirty = false;
	        }

	        CCPoint selStart = ccp(getContentWidth() - 20,0);
	        ccDrawSolidRect(selStart, getContentSize(), ccc4FFromccc3B(m_selColor));

	        m_carat.setPosition(ccpMidpoint(selStart, getContentSize()));
	        m_carat.visit();

	        auto chosen = m_labels.getChildrenExt()[m_signal->get().value_or(0)];
	        if (chosen) {
	            chosen->getChildByIndex(0)->setPositionY(getContentHeight() / 2);
	            chosen->getChildByIndex(0)->visit();
	        }

	        if (m_open) {
	            auto pos = convertToNodeSpace(cocos::getMousePos());
	            pos.y += m_labels.getContentHeight();
	            for (auto& child : m_labels.getChildrenExt<CCMenuColor*>()) {
	                if (child->boundingBox().containsPoint(pos)) {
	                    child->setColor(m_selColor);
	                } else {
	                    child->setColor(getColor());
	                }
	            }

	            m_labels.setContentWidth(getContentWidth() - 20);
	            m_labels.visit();
	        }
	    }

	    bool ccTouchBegan(CCTouch* touch, CCEvent* event) override {
	        auto pos = convertTouchToNodeSpace(touch);

	        if (m_open) {
	            pos.y += m_labels.getContentHeight();
	            int i = 0;
	            for (auto& child : m_labels.getChildrenExt<CCMenuColor*>()) {
	                if (child->boundingBox().containsPoint(pos)) {
	                    *m_signal = i;
	                }
	                ++i;
	            }
	            m_open = false;
	            return true;
	        } else if (CCRect(CCPointZero, getContentSize()).containsPoint(pos)) {
	            m_open = true;
	            return true;
	        } else {
	            return false;
	        }
	    }
	};
}