#include "2d/CCAction.h"
#include "2d/CCActionEase.h"
#include "2d/CCActionInterval.h"
#include "2d/CCMenu.h"
#include "math/CCGeometry.h"

template <typename T> class Singleton {
  public:
    static T *getInstance() {
        static T *inst = new T();
        return inst;
    }
};

class MenuState {
    cocos2d::Menu *_menu;

  public:
    virtual ~MenuState() {
	CC_SAFE_DELETE(_menu);
    };
    virtual MenuState *swipeRight() { return this; };
    virtual MenuState *swipeLeft() { return this; };
    virtual MenuState *swipeUpwards() { return this; };
    virtual MenuState *swipeDownwards() { return this; };
    virtual void setMoveToDims(cocos2d::Rect& visRect) = 0;

    void setNode(cocos2d::Menu *menu, cocos2d::Rect& visRect) {
	_menu = menu; 
	setMoveToDims(visRect);
    };
    cocos2d::Menu *getMenu() { return _menu; };
};

class CenterState : public MenuState, public Singleton<CenterState> {
    cocos2d::MoveBy *actionLeft;
    cocos2d::EaseBounceOut *easeLeft;
    cocos2d::MoveBy *actionRight;
    cocos2d::EaseBounceOut *easeRight;
    cocos2d::MoveBy *actionUp;
    cocos2d::EaseBounceOut *easeUp;
    CenterState();
    virtual ~CenterState() override;

  public:
    virtual MenuState *swipeRight() override;
    virtual MenuState *swipeLeft() override;
    virtual MenuState *swipeUpwards() override;
    virtual void setMoveToDims(cocos2d::Rect& visRect) override;
    friend class Singleton<CenterState>;
};

class LeftState : public MenuState, public Singleton<LeftState> {
    cocos2d::MoveBy *actionRight;
    cocos2d::EaseBounceOut *easeRight;
    LeftState();
    virtual ~LeftState() override;

  public:
    virtual MenuState *swipeRight() override;
    virtual void setMoveToDims(cocos2d::Rect& visRect) override;
    friend class Singleton<LeftState>;
};

class RightState : public MenuState, public Singleton<RightState> {
    cocos2d::MoveBy *actionLeft;
    cocos2d::EaseBounceOut *easeLeft;
    RightState();
    virtual ~RightState() override;

  public:
    virtual MenuState *swipeLeft() override;
    virtual void setMoveToDims(cocos2d::Rect& visRect) override;
    friend class Singleton<RightState>;
};

class InActiveState : public MenuState, public Singleton<InActiveState> {
    cocos2d::MoveBy *actionDown;
    cocos2d::EaseBounceOut *easeDown;
    InActiveState();
    ~InActiveState();

  public:
    virtual MenuState *swipeDownwards() override;
    virtual void setMoveToDims(cocos2d::Rect& visRect) override;
    friend class Singleton<InActiveState>;
};

class MenuDelegate {
    MenuState *_currState;

  public:
    void setNodeAndSize(cocos2d::Menu *menu, cocos2d::Rect& visRect) {
        InActiveState::getInstance()->setNode(menu, visRect);
	CenterState::getInstance()->setNode(menu, visRect);
	LeftState::getInstance()->setNode(menu, visRect);
	RightState::getInstance()->setNode(menu, visRect);

        _currState = InActiveState::getInstance();
    };
    void swipeRight() { _currState = _currState->swipeRight(); };
    void swipeLeft() { _currState = _currState->swipeLeft(); };
    void swipeUpwards() { _currState = _currState->swipeUpwards(); };
    void swipeDownwards() { _currState = _currState->swipeDownwards(); };
};
