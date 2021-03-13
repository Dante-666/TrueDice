#include "CCZoomMenuImage.h"
#include "2d/CCAction.h"
#include "2d/CCActionInterval.h"
#include "2d/CCSprite.h"

NS_CC_BEGIN

const unsigned int kZoomActionTag = 0xc0c05012;

MenuItemZoomImage *MenuItemZoomImage::create(const std::string &normalImage,
                                             const std::string &selectedImage) {
    MenuItemZoomImage *ret = new (std::nothrow) MenuItemZoomImage();
    if (ret && ret->initWithNormalImage(normalImage, selectedImage)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
};

bool MenuItemZoomImage::initWithNormalImage(const std::string &normalImage,
                                            const std::string &selectedImage) {
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
    if (selectedImage.size() > 0) {
        img = Sprite::create(selectedImage);
        if (img) {
            addChild(img);
            img->setAnchorPoint(Vec2(0, 0));
        }
        if (_selectedImage) {
            removeChild(_selectedImage, true);
        }
        _selectedImage = img;
        if (_selectedImage) {
            _selectedImage->setVisible(true);
            _selectedImage->setOpacity(0);
            setAnchorPoint(Vec2(0.5, 0.5));
        }
    }
    setCascadeColorEnabled(true);
    setCascadeOpacityEnabled(true);
    return true;
};

bool MenuItemZoomImage::isSelected() { return _selected; }

void MenuItemZoomImage::selectImage() {
    Action *fadeAction = FadeTo::create(0.3f, 180);
    _selectedImage->runAction(fadeAction);
    _selected = true;
}

void MenuItemZoomImage::unSelectImage() {
    Action *fadeAction = FadeTo::create(0.3f, 0);
    _selectedImage->runAction(fadeAction);
    _selected = false;
}

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
