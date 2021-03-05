#include "2d/CCAction.h"
#include "2d/CCActionEase.h"
#include "2d/CCActionInterval.h"
#include "2d/CCMenu.h"

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

    void setMenu(cocos2d::Menu *menu) {
	_menu = menu; 
	CC_SAFE_RETAIN(_menu);
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
    friend class Singleton<CenterState>;
};

class LeftState : public MenuState, public Singleton<LeftState> {
    cocos2d::MoveBy *actionRight;
    cocos2d::EaseBounceOut *easeRight;
    LeftState();
    virtual ~LeftState() override;

  public:
    virtual MenuState *swipeRight() override;
    friend class Singleton<LeftState>;
};

class RightState : public MenuState, public Singleton<RightState> {
    cocos2d::MoveBy *actionLeft;
    cocos2d::EaseBounceOut *easeLeft;
    RightState();
    virtual ~RightState() override;

  public:
    virtual MenuState *swipeLeft() override;
    friend class Singleton<RightState>;
};

class InActiveState : public MenuState, public Singleton<InActiveState> {
    cocos2d::MoveBy *actionDown;
    cocos2d::EaseBounceOut *easeDown;
    InActiveState();
    ~InActiveState();

  public:
    virtual MenuState *swipeDownwards() override;
    friend class Singleton<InActiveState>;
};

class MenuDelegate {
    MenuState *_currState;

  public:
    void setMenu(cocos2d::Menu *menu) {
        InActiveState::getInstance()->setMenu(menu);
	CenterState::getInstance()->setMenu(menu);
	LeftState::getInstance()->setMenu(menu);
	RightState::getInstance()->setMenu(menu);

        _currState = InActiveState::getInstance();
    };
    void swipeRight() { _currState = _currState->swipeRight(); };
    void swipeLeft() { _currState = _currState->swipeLeft(); };
    void swipeUpwards() { _currState = _currState->swipeUpwards(); };
    void swipeDownwards() { _currState = _currState->swipeDownwards(); };
};
