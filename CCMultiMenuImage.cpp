#include "CCMultiMenuImage.h"
#include "2d/CCAction.h"
#include "2d/CCActionInterval.h"
#include "2d/CCSprite.h"

NS_CC_BEGIN

const unsigned int kMultiActionTag = 0xc0c05012;

MenuItemMultiImage *MenuItemMultiImage::create(const std::string &img) {
    MenuItemMultiImage *ret = new (std::nothrow) MenuItemMultiImage();
    if (ret && ret->insertImage(img) && ret->insertColors()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
};

bool MenuItemMultiImage::insertImage(const std::string &img) {
    Node *currImg = nullptr;
    if (img.size() > 0) {
        currImg = Sprite::create(img);
        if (currImg) {
            addChild(currImg);
            currImg->setAnchorPoint(Vec2(0, 0));
            this->setContentSize(currImg->getContentSize());
            if (_enabled) {
                currImg->setVisible(true);
            }
            _imgList.push_back(currImg);
            if (_currIdx >= 0) {
                _imgList[_currIdx]->setOpacity(0);
            }
            _currIdx++;
            if (_currIdx == 0) {
                setAnchorPoint(Vec2(0.5, 0.5));
                setCascadeColorEnabled(true);
                setCascadeOpacityEnabled(true);
            }
        }
    }
    return true;
};

bool MenuItemMultiImage::insertColors() {
    // Sequence should be RGB, starting from R
    _colorList.push_back(Color4F(Color3B(0, 240, 40)));
    _colorList.push_back(Color4F(Color3B(0, 40, 240)));
    _colorList.push_back(Color4F(Color3B(240, 40, 0)));
    return true;
};

bool MenuItemMultiImage::activateNextImage() {
    if (_enabled) {
        CC_ASSERT(_imgList.size() >= 1);
        Action *fadeOut = FadeOut::create(0.2f);
        Action *fadeIn = FadeIn::create(0.15f);
        _imgList[_currIdx]->runAction(fadeOut);
        // Implement circularity
        if (_currIdx + 1 >= _imgList.size()) {
            _currIdx = 0;
        } else {
            _currIdx++;
        }
        _imgList[_currIdx]->runAction(fadeIn);
        return true;
    }
    return false;
}

const Color4F &MenuItemMultiImage::getColor() { return _colorList[_currIdx]; }

void MenuItemMultiImage::selected() {
    if (_enabled) {
        MenuItem::selected();

        Action *action = getActionByTag(kMultiActionTag);
        if (action) {
            this->stopAction(action);
        } else {
            _originalScale = this->getScale();
        }

        Action *zoomAction = ScaleTo::create(0.1f, _originalScale * 1.2f);
        zoomAction->setTag(kMultiActionTag);
        this->runAction(zoomAction);
    }
}

void MenuItemMultiImage::unselected() {
    if (_enabled) {
        MenuItem::unselected();
        this->stopActionByTag(kMultiActionTag);
        Action *zoomAction = ScaleTo::create(0.1f, _originalScale);
        zoomAction->setTag(kMultiActionTag);
        this->runAction(zoomAction);
    }
}

NS_CC_END
