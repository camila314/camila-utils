#pragma once
#include <Geode/utils/cocos.hpp>

namespace camila {
	using namespace geode::prelude;

	class Copy {
	 public:
		template <typename T>
		static T* copy(T* src, T* dest) { return src->copy(dest); }
	};

	template <std::derived_from<cocos2d::CCObject> T>
	T* copy(T* src, T* dest = nullptr) {
		if (dest == nullptr) {
			if constexpr (std::same_as<T, AxisLayout>)
				dest = AxisLayout::create();
			else if constexpr (std::same_as<T, AxisLayoutOptions>)
				dest = AxisLayoutOptions::create();
			else if constexpr (std::same_as<T, AnchorLayout>)
				dest = AnchorLayout::create();
			else if constexpr (std::same_as<T, AnchorLayoutOptions>)
				dest = AnchorLayoutOptions::create();
			else if constexpr (std::same_as<T, CCNode>)
				dest = CCNode::create();
			else if constexpr (std::same_as<T, CCTexture2D>)
				dest = new CCTexture2D;
			else if constexpr (std::same_as<T, CCSprite>)
				dest = CCSprite::create();
			else if constexpr (std::same_as<T, CCLabelBMFont>)
				dest = CCLabelBMFont::create(src->getString(), src->getFntFile());
			else if constexpr (requires(T* a, T* b) { a->copy(b); })
				return Copy::copy(src, dest);
			else
				return nullptr;
		}

		if constexpr (std::derived_from<T, Layout>) {
			dest->ignoreInvisibleChildren(src->ignoresInvisibleChildren());
		}
		if constexpr (std::derived_from<T, AxisLayout>) {
			dest->setAxis(src->getAxis());
			dest->setGap(src->getGap());
			dest->setAxisAlignment(src->getAxisAlignment());
			dest->setCrossAxisAlignment(src->getCrossAxisAlignment());
			dest->setCrossAxisLineAlignment(src->getCrossAxisLineAlignment());
			dest->setAxisReverse(src->isAxisReverse());
			dest->setCrossAxisReverse(src->isCrossAxisReverse());
			dest->setAutoScale(src->getAutoScale());
			dest->setGrowCrossAxis(src->getGrowCrossAxis());
			dest->setCrossAxisOverflow(src->getCrossAxisOverflow());
			dest->setAutoGrowAxis(src->getAutoGrowAxis());
			dest->setDefaultScaleLimits(
				src->getDefaultMinScale(),
				src->getDefaultMaxScale()
			);
		}
		if constexpr (std::derived_from<T, AxisLayoutOptions>) {
			dest->setScaleLimits(
				src->getMinScale(),
				src->getMaxScale()
			);
			dest->setRelativeScale(src->getRelativeScale());
			dest->setAutoScale(src->getAutoScale());
			dest->setLength(src->getLength());
			dest->setPrevGap(src->getPrevGap());
			dest->setNextGap(src->getNextGap());
			dest->setBreakLine(src->getBreakLine());
			dest->setSameLine(src->getSameLine());
			dest->setScalePriority(src->getScalePriority());
			dest->setCrossAxisAlignment(src->getCrossAxisAlignment());
		}
		if constexpr (std::derived_from<T, AnchorLayoutOptions>) {
			dest->setAnchor(src->getAnchor());
			dest->setOffset(src->getOffset());
		}
		if constexpr (std::derived_from<T, CCNode>) {
			dest->setPosition(src->getPosition());
			dest->setScaleX(src->getScaleX());
			dest->setScaleY(src->getScaleY());
			dest->setRotation(src->getRotation());
			dest->setSkewX(src->getSkewX());
			dest->setSkewY(src->getSkewY());
			dest->ignoreAnchorPointForPosition(src->isIgnoreAnchorPointForPosition());
			dest->setAnchorPoint(src->getAnchorPoint());
			dest->setContentSize(src->getContentSize());
			dest->setVisible(src->isVisible());
			dest->setZOrder(src->getZOrder());
			dest->setTag(src->getTag());
			dest->setID(src->getID());
			dest->setLayout(copy(src->getLayout()));
			dest->setLayoutOptions(copy(src->getLayoutOptions()));
		}
		if constexpr (std::derived_from<T, CCRGBAProtocol>) {
			dest->setOpacity(src->getOpacity());
			dest->setColor(src->getColor());
			dest->setCascadeColorEnabled(src->isCascadeColorEnabled());
			dest->setCascadeOpacityEnabled(src->isCascadeOpacityEnabled());
			dest->setOpacityModifyRGB(src->isOpacityModifyRGB());
		}
		if constexpr (std::derived_from<T, CCTexture2D>) {
			if (!dest) {
				if (std::same_as<T, CCTexture2D>)
					dest = new CCTexture2D;
				else
					return nullptr;
			}

			dest->initWithData(
				src->getData(),
				src->getPixelFormat(),
				src->getPixelsWide(),
				src->getPixelsHigh(),
				src->getContentSize()
			);
		}
		if constexpr (std::derived_from<T, CCSprite>) {
			dest->setTexture(copy(src->getTexture()));
			dest->setTextureRect(src->getTextureRect());
			dest->setBlendFunc(src->getBlendFunc());
			dest->setFlipX(src->isFlipX());
			dest->setFlipY(src->isFlipY());
			dest->setDisplayFrame(src->displayFrame());
			dest->setTextureAtlas(src->getTextureAtlas());
		}
		if constexpr (std::derived_from<T, CCLabelBMFont>) {
			dest->setString(src->getString());
			dest->setFntFile(src->getFntFile());
			dest->setAlignment(src->m_pAlignment);
			dest->setWidth(src->m_fWidth);
			dest->setExtraKerning(src->getExtraKerning());
			dest->setLineBreakWithoutSpace(src->m_bLineBreakWithoutSpaces);
		}
		if constexpr (requires(T* a, T* b) { a->copy(b); }) {
			Copy::copy(src, dest);
		}

		return dest;
	}
};
