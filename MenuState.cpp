#include "MenuState.h"
#include "2d/CCActionEase.h"
#include "2d/CCActionInterval.h"
#include "math/Vec2.h"

CenterState::CenterState() {
    actionUp = cocos2d::MoveBy::create(1, cocos2d::Vec2(-300, 0));
    easeUp = cocos2d::EaseBounceOut::create(actionUp);
    CC_SAFE_RETAIN(easeUp);
    actionLeft = cocos2d::MoveBy::create(1, cocos2d::Vec2(0, -300));
    easeLeft = cocos2d::EaseBounceOut::create(actionLeft);
    CC_SAFE_RETAIN(easeLeft);
    actionRight = cocos2d::MoveBy::create(1, cocos2d::Vec2(0, 300));
    easeRight = cocos2d::EaseBounceOut::create(actionRight);
    CC_SAFE_RETAIN(easeRight);
}
CenterState::~CenterState() {
    delete easeUp;
    delete actionUp;
    delete easeLeft;
    delete actionLeft;
    delete easeRight;
    delete actionRight;
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

LeftState::LeftState() {
    actionRight = cocos2d::MoveBy::create(1, cocos2d::Vec2(0, 300));
    easeRight = cocos2d::EaseBounceOut::create(actionRight);
    CC_SAFE_RETAIN(easeRight);
}

LeftState::~LeftState() {
    delete easeRight;
    delete actionRight;
}

MenuState *LeftState::swipeRight() {
    getMenu()->runAction(easeRight);
    return CenterState::getInstance();
}

RightState::RightState() {
    actionLeft = cocos2d::MoveBy::create(1, cocos2d::Vec2(0, -300));
    easeLeft = cocos2d::EaseBounceOut::create(actionLeft);
    CC_SAFE_RETAIN(easeLeft);
}

RightState::~RightState() {
    delete easeLeft;
    delete actionLeft;
}

MenuState *RightState::swipeLeft() {
    getMenu()->runAction(easeLeft);
    return CenterState::getInstance();
}

InActiveState::InActiveState() {
    actionDown = cocos2d::MoveBy::create(1, cocos2d::Vec2(300, 0));
    easeDown = cocos2d::EaseBounceOut::create(actionDown);
    CC_SAFE_RETAIN(easeDown);
}
InActiveState::~InActiveState() {
    delete easeDown;
    delete actionDown;
}
MenuState *InActiveState::swipeDownwards() {
    getMenu()->runAction(easeDown);
    return CenterState::getInstance();
};
