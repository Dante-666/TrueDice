#include "MenuState.h"
#include "2d/CCActionEase.h"
#include "2d/CCActionInterval.h"
#include "math/CCGeometry.h"
#include "math/Vec2.h"

CenterState::CenterState() {}

CenterState::~CenterState() {
    CC_SAFE_DELETE(easeUp);
    CC_SAFE_DELETE(actionUp);
    CC_SAFE_DELETE(easeLeft);
    CC_SAFE_DELETE(actionLeft);
    CC_SAFE_DELETE(easeRight);
    CC_SAFE_DELETE(actionRight);
}
MenuState *CenterState::swipeRight() {
    getMenu()->runAction(easeRight);
    return RightState::getInstance();
};
MenuState *CenterState::swipeLeft() {
    getMenu()->runAction(easeLeft);
    return LeftState::getInstance();
};
MenuState *CenterState::swipeUpwards() {
    getMenu()->runAction(easeUp);
    return InActiveState::getInstance();
};
void CenterState::setMoveToDims(cocos2d::Rect &visRect) {
    actionUp =
        cocos2d::MoveBy::create(0.75, cocos2d::Vec2(-visRect.size.width, 0));
    easeUp = cocos2d::EaseCubicActionOut::create(actionUp);
    CC_SAFE_RETAIN(easeUp);
    actionLeft =
        cocos2d::MoveBy::create(0.75, cocos2d::Vec2(0, -visRect.size.height));
    easeLeft = cocos2d::EaseCubicActionOut::create(actionLeft);
    CC_SAFE_RETAIN(easeLeft);
    actionRight =
        cocos2d::MoveBy::create(0.75, cocos2d::Vec2(0, visRect.size.height));
    easeRight = cocos2d::EaseCubicActionOut::create(actionRight);
    CC_SAFE_RETAIN(easeRight);
}

LeftState::LeftState() {}

LeftState::~LeftState() {
    CC_SAFE_DELETE(easeRight);
    CC_SAFE_DELETE(actionRight);
}
MenuState *LeftState::swipeRight() {
    getMenu()->runAction(easeRight);
    return CenterState::getInstance();
}
void LeftState::setMoveToDims(cocos2d::Rect &visRect) {
    actionRight =
        cocos2d::MoveBy::create(0.75, cocos2d::Vec2(0, visRect.size.height));
    easeRight = cocos2d::EaseCubicActionOut::create(actionRight);
    CC_SAFE_RETAIN(easeRight);
}

RightState::RightState() {}

RightState::~RightState() {
    CC_SAFE_DELETE(easeLeft);
    CC_SAFE_DELETE(actionLeft);
}

MenuState *RightState::swipeLeft() {
    getMenu()->runAction(easeLeft);
    return CenterState::getInstance();
}
void RightState::setMoveToDims(cocos2d::Rect &visRect) {
    actionLeft =
        cocos2d::MoveBy::create(0.75, cocos2d::Vec2(0, -visRect.size.height));
    easeLeft = cocos2d::EaseCubicActionOut::create(actionLeft);
    CC_SAFE_RETAIN(easeLeft);
}

InActiveState::InActiveState() {}

InActiveState::~InActiveState() {
    CC_SAFE_DELETE(easeDown);
    CC_SAFE_DELETE(actionDown);
}
MenuState *InActiveState::swipeDownwards() {
    getMenu()->runAction(easeDown);
    return CenterState::getInstance();
};
void InActiveState::setMoveToDims(cocos2d::Rect &visRect) {
    actionDown =
        cocos2d::MoveBy::create(1, cocos2d::Vec2(visRect.size.width, 0));
    easeDown = cocos2d::EaseBounceOut::create(actionDown);
    CC_SAFE_RETAIN(easeDown);
}
