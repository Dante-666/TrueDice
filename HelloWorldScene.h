/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "2d/CCLabelAtlas.h"
#include "CCEvent.h"
#include "CCEventCustom.h"
#include "CCEventListenerCustom.h"
#include "cocos2d.h"

#include "AudioEngine.h"
#include "math/Vec3.h"
#include "physics3d/CCPhysics3D.h"
#include "physics3d/CCPhysics3DObject.h"

#include "MenuState.h"
#include <vector>

#define TG3_GRAVITY_EARTH (9.80665f)

class HelloWorld : public cocos2d::Scene {
    cocos2d::Mat4 android2Game;
    // This should be a vector of dices when multiple
    // will be involved
    cocos2d::Physics3DRigidBody *dice;

#ifdef COCOS2D_DEBUG
    cocos2d::Label *accelLabel;
    cocos2d::Label *gravityLabel;
    float _accumlaDt = 0;
    float _accumgDt = 0;
#endif

    // Menu specific items
    MenuDelegate _menuD;
    cocos2d::Menu* _menu;

    // Store initial touch location
    cocos2d::Vec2* _touchBeginLoc;
    float _minSwipeLen = 0.4;

  public:
    static cocos2d::Scene *createScene();

    virtual bool init() override;

    // add menu item and callback for slide
    void insertMainMenu();

#ifdef COCOS2D_DEBUG
    // display acceleration data for debug
    void insertDebugLabels();
#endif

    // include physics and camera node
    void initPhysicsAndCamera();

    // right now manipulate the member, make this return
    // a rigid body so that vector can be maintained later
    void insertDice();

    // add a box composed with places
    void addQBox();

    // attach acceleration event callbacks
    void createAccelerationCallbacks();

    // return a vector of surface points for convex hull
    std::vector<std::vector<cocos2d::Vec3>> getQuadPlanes();

  private:
    // callback for touch and swipe events
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);

    void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);

    // callback for processing acceleration
    void onAcceleration(cocos2d::Acceleration *acc, cocos2d::Event *event);

    // callback for processing gravity
    void onGravity(cocos2d::EventCustom *event);

    // a selector callback
    void menuCloseCallback(cocos2d::Ref *pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
