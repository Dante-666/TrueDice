#include "CCZoomMenuImage.h"
#include "2d/CCAction.h"
#include "2d/CCActionInterval.h"
#include "2d/CCSprite.h"

NS_CC_BEGIN

const unsigned int kZoomActionTag = 0xc0c05012;

MenuItemZoomImage *MenuItemZoomImage::create(const std::string &normalImage) {
    MenuItemZoomImage *ret = new (std::nothrow) MenuItemZoomImage();
    if (ret && ret->initWithNormalImage(normalImage)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
};

bool MenuItemZoomImage::initWithNormalImage(const std::string &normalImage) {
    Node *img = nullptr;
    if (normalImage.size() > 0) {
        img = Sprite::create(normalImage);
        if (img) {
            addChild(img);
            img->setAnchorPoint(Vec2(0, 0));
        }
        if (_normalImage) {
            removeChild(_normalImage, true);
        }
        _normalImage = img;
        if (_normalImage) {
            this->setContentSize(_normalImage->getContentSize());
            if (_enabled) {
                _normalImage->setVisible(true);
            }
	    setAnchorPoint(Vec2(0.5, 0.5));
        }
    }
    setCascadeColorEnabled(true);
    setCascadeOpacityEnabled(true);
    return true;
};

void MenuItemZoomImage::selected() {
    if (_enabled) {
        MenuItem::selected();

        Action *action = getActionByTag(kZoomActionTag);
        if (action) {
            this->stopAction(action);
        } else {
            _originalScale = this->getScale();
        }

        Action *zoomAction = ScaleTo::create(0.1f, _originalScale * 1.2f);
        zoomAction->setTag(kZoomActionTag);
        this->runAction(zoomAction);
    }
}

void MenuItemZoomImage::unselected() {
    if (_enabled) {
        MenuItem::unselected();
        this->stopActionByTag(kZoomActionTag);
        Action *zoomAction = ScaleTo::create(0.1f, _originalScale);
        zoomAction->setTag(kZoomActionTag);
        this->runAction(zoomAction);
    }
}

NS_CC_END
