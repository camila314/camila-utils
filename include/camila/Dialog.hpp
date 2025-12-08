#pragma once

#include <Geode/utils/Task.hpp>
#include <Geode/utils/cocos.hpp>
#include <Geode/UI.hpp>
#include <UIBuilder.hpp>
#include "CCMenuColor.hpp"

namespace camila {
	using namespace geode::prelude;

	template <typename T, typename P = std::monostate> requires std::constructible_from<Task<T, P>>
	class Dialog : public CCMenuColor {
	 protected:
	    using TaskType = Task<T, P>;
	 private:
	    TaskType m_task;
	    TaskType::PostResult m_postResult;
	    TaskType::PostProgress m_postProgress;
	 protected:

	    T m_finishValue;

	    template <std::convertible_to<T> Q>
	    void finish(Q&& value) {
	        m_postResult(std::forward<Q>(value));
	        removeFromParentAndCleanup(true);
	    }

	    void finish() {
	        finish(std::move(m_finishValue));
	    }

	    template <std::convertible_to<P> Q>
	    void progress(Q&& value) {
	        m_postProgress(std::forward<Q>(value));
	    }

	    template <std::convertible_to<T> Q>
	    Dialog(Q&& finishVal) : m_finishValue(finishVal) {
	        CCMenuColor::init();

	        setColor(ccc3(0, 0, 0));
	        setOpacity(150);
	        setPosition(0, 0);

	        registerWithTouchDispatcher();
	        CCDirector::sharedDirector()->getTouchDispatcher()->registerForcePrio(this, 2);

	        auto [task, post, prog, cancelled] = TaskType::spawn();

	        m_task = std::move(task);
	        m_postResult = std::move(post);
	        m_postProgress = std::move(prog);

	        autorelease();
	    }

	    Dialog() requires std::constructible_from<T> : Dialog(T{}) {}
	 public:
	    virtual void show() {
	        CCDirector::sharedDirector()->getRunningScene()->addChild(this, 99999);
	        setCascadeOpacityEnabled(false);
	        setOpacity(0);
	        runAction(CCFadeTo::create(0.14, 150));
	    }

	    TaskType task() {
	        return m_task;
	    }
	};


	class Confirmation : public Dialog<bool> {
	 public:
	    struct Builder {
	        std::optional<const char*> title;
	        std::optional<const char*> sprite;
	        std::optional<const char*> yesBtn;
	        std::optional<const char*> noBtn;
	        std::optional<float> width;
	    };

	    void show() override {
	        Dialog<bool>::show();

	        auto layout = getChildByID("layout");
	        auto sprite = getChildByID("sprite");

	        if (layout && sprite) {
	            layout->setScale(0.1);
	            sprite->setScale(0.1);

	            layout->runAction(CCEaseElasticOut::create(CCScaleTo::create(0.5, 1.0), 0.6));
	            sprite->runAction(CCEaseElasticOut::create(CCScaleTo::create(0.5, 1.0), 0.6));
	        }
	    }

	    Confirmation(
	        std::string const& message,
	        char const* title,
	        char const* sprite,
	        char const* yesBtn,
	        char const* noBtn,
	        float width
	    ) : Dialog() {
	        CCNode* node;

	        Build<CCNode>::create()
	            .id("layout")
	            .store(node)
	            .layout(Build<ColumnLayout>::create().gap(35).align(AxisAlignment::Center).reverse(true).autoGrow(0))
	            .anchorPoint({0.5, 0.5})
	            .children(
	                Build<CCLabelBMFont>::create(title, "goldFont.fnt")
	                    .id("title")
	                    .scale(0.8f),
	                Build<TextArea>::create(message, "chatFont.fnt", 1.0, 300, ccp(0.5, 0.5), 1, false)
	                    .id("message"),
	                Build<CCMenu>::create()
	                    .id("button-layout")
	                    .layout(Build<RowLayout>::create().align(AxisAlignment::Between).crossGrow(true))
	                    .contentSize(200, 100)
	                    .children(
	                        Build<ButtonSprite>::create(noBtn)
	                            .intoMenuItem([&] { this->finish(false); })
	                            .id("no-btn"),
	                        Build<ButtonSprite>::create(yesBtn)
	                            .intoMenuItem([&] { this->finish(true); })
	                            .id("yes-btn"))
	                    .updateLayout())
	            .parent(this)
	            .zOrder(2)
	            .center()
	            .updateLayout();

	        Build<CCScale9Sprite>::create(sprite)
	            .id("sprite")
	            .contentSize(width, node->getContentHeight() + 30)
	            .parent(this)
	            .zOrder(1)
	            .center();
	    }

	    /**
	     * Builder options:
	     * char const* title (default: "Confirm")
	     * 
	     * char const* sprite (default: "square01_001.png")
	     * 
	     * char const* yesBtn (default: "Yes")
	     * 
	     * char const* noBtn (default: "No")
	     * 
	     * float width (default: 284.5)
	     */
	    static Confirmation* create(std::string const& message, Builder&& builder = Builder()) {
	        return new Confirmation(
	            message,
	            builder.title.value_or("Confirm"),
	            builder.sprite.value_or("square01_001.png"),
	            builder.yesBtn.value_or("Yes"),
	            builder.noBtn.value_or("No"),
	            builder.width.value_or(284.5)
	        );
	    }
	};
}