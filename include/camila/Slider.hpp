#pragma once

#include <Geode/utils/cocos.hpp>
#include <Geode/UI.hpp>
#include <UIBuilder.hpp>
#include "reactive.hpp"

namespace camila {
	using namespace geode::prelude;

	class Slider : public CCLayer {
		inline static ccTexParams s_params = {
		    GL_LINEAR, GL_LINEAR,
		    GL_REPEAT, GL_CLAMP_TO_EDGE
		};

		// can be initialized
		std::pair<double, double> m_bounds = {0, 100};
		RefSignal<double> m_signal;
		Ref<CCScale9Sprite> m_frame = CCScale9Sprite::create("slidergroove.png");
		Ref<CCSprite> m_knob = CCSprite::create("sliderthumb.png");
		Ref<CCSprite> m_knobSelect = CCSprite::create("sliderthumbsel.png");
		Ref<CCSprite> m_fill = CCSprite::create("sliderBar.png");
		double m_gap = 3;

		// internal state
		bool m_dirty = true;
		bool m_holding = false;
	 public:
		Slider(RefSignal<double> ref) : m_signal(ref) {
			CCLayer::init();
			autorelease();

			Build(this)
				.contentSize(210, 16)
				.ignoreAnchorPointForPos(false)
				.initTouch();

			m_signal->react([&](double v) {
				if (v > m_bounds.second) [[unlikely]] {
					*m_signal = m_bounds.second;
				} else if (v < m_bounds.first) [[unlikely]] {
					*m_signal = m_bounds.first;
				}

				double ratio = (v - m_bounds.first) / (m_bounds.second - m_bounds.first);
				float distance = ratio * (getContentWidth() - m_gap * 2);

				m_knob->setPositionX(m_gap + distance);
				m_knobSelect->setPositionX(m_gap + distance);
				m_fill->setContentWidth(distance);

				auto rect = m_fill->getTextureRect();
				rect.size.width = distance;
				m_fill->setTextureRect(rect);
			});
		}

		static Slider* create(RefSignal<double> ref) {
			return new Slider(ref);
		}
	
		auto thumb(CCSprite* knob, CCSprite* knobSelect) {
			m_knob = knob;
			m_knobSelect = knobSelect;
			m_dirty = true;
			return Build(this);
		}

		auto frame(CCScale9Sprite* frame) {
			m_frame = frame;
			m_dirty = true;
			return Build(this);
		}

		auto bar(CCSprite* fill) {
			m_fill = fill;
			m_dirty = true;
			return Build(this);
		}

		auto bounds(double min, double max) {
			m_bounds = {min, max};
			return Build(this);
		}

		auto gap(double gap) {
			m_gap = gap;
			m_dirty = true;
			return Build(this);
		}

		void setContentSize(const CCSize& size) override {
			CCNode::setContentSize(size);
			m_dirty = true;
		}

		void draw() override {
			CCNode::draw();

			if (m_dirty) [[unlikely]] {
				setContentHeight(m_frame->getContentHeight());

				m_frame->setAnchorPoint({0, 0});
				m_fill->setAnchorPoint({0, 0.5});
				m_frame->setContentWidth(getContentWidth());

				float center = m_frame->getContentHeight() / 2;
				m_knob->setPositionY(center);
				m_knobSelect->setPositionY(center);

				m_fill->setPosition(ccp(m_gap, center));
				m_fill->getTexture()->setTexParameters(&s_params);

				*m_signal = m_signal->get().value_or(0);
				m_dirty = false;
			}

			m_fill->visit();
			m_frame->visit();

			if (m_holding) [[unlikely]]
				m_knobSelect->visit();
			else
				m_knob->visit();
		}

		bool ccTouchBegan(CCTouch* touch, CCEvent* event) override {
			auto loc = m_knob->convertToNodeSpace(convertTouchToNodeSpace(touch));

			if (CCRect(CCPointZero, m_knob->getContentSize()).containsPoint(loc)) {
				m_holding = true;
				return true;
			}

			return false;
		}

		void ccTouchMoved(CCTouch* touch, CCEvent* event) override {
			if (!m_holding) return;

			auto start = touch->getPreviousLocation();
			auto curr = touch->getLocation();
			double computed = m_bounds.second * (m_knob->getPositionX() + (curr.x - start.x) - m_gap) / (getContentWidth() - m_gap * 2);

			auto loc = m_frame->convertToNodeSpace(convertTouchToNodeSpace(touch));
			if (loc.x > m_frame->getContentWidth() - m_gap && computed < m_signal->get())
				return;
			if (loc.x < m_gap && computed > m_signal->get())
				return;

			*m_signal = std::clamp(computed, m_bounds.first, m_bounds.second);
		}

		void ccTouchEnded(CCTouch* touch, CCEvent* event) override {
			m_holding = false;
		}
	};
}
