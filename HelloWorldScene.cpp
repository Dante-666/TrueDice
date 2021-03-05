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

#include "HelloWorldScene.h"
#include "renderer/backend/Types.h"
#include <vector>
#include <iostream>

USING_NS_CC;

Scene *HelloWorld::createScene() { return HelloWorld::create(); }

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char *filename) {
    printf("Error while loading: %s\n", filename);
    printf(
        "Depending on how you compiled you might have to add 'Resources/' in "
        "front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init() {
    //////////////////////////////
    // 1. super init first
    if (!Scene::initWithPhysics()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
        "CloseNormal.png", "CloseSelected.png",
        CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    if (closeItem == nullptr || closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0) {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    } else {
        float x = origin.x + visibleSize.width -
                  closeItem->getContentSize().width / 2;
        float y = origin.y + closeItem->getContentSize().height / 2;
        closeItem->setPosition(Vec2(x, y));
    }

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    insertMainMenu();

#ifdef COCOS2D_DEBUG
    insertDebugLabels();
#endif

    initPhysicsAndCamera();

    insertDice();

    // generic idea would be to take the camera furstum and intersect it with
    // two planes and figure out the extremities of the box and calculate the
    // actual box
    addQBox();

    createAccelerationCallbacks();

    return true;
}

void HelloWorld::insertMainMenu() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

#ifdef COCOS2D_DEBUG
    auto printVec = [] (Vec2& vec) {
	std::cout << "Vector : (" << vec.x << ", " << vec.y << ")" << std::endl;
    };
    auto printSize = [] (Size& sz) {
	std::cout << "Size : (" << sz.width << ", " << sz.height << ")" << std::endl;
    };
    auto printRect = [](Rect &rt) {
        std::cout << "Rect : (" << rt.origin.x << ", " << rt.origin.y << " | "
                  << rt.size.width << ", " << rt.size.height << ")" << std::endl;
    };
#endif

#ifdef COCOS2D_DEBUG
    printVec(origin);
    printSize(visibleSize);
#endif

    // This gives how much screen area we have to work with
    auto halfRect = Rect(origin, visibleSize);
    halfRect.size.width /= 2;

    // Calculate the bounding rects of add, close and refresh buttons
    auto w1_3 = halfRect.size.width/3;
    auto botRectOrigin = Vec2(halfRect.size.width - w1_3, 0);
    auto botRectOffset = Vec2(0, w1_3);
    auto botRectSize = Size(w1_3, w1_3);

    auto addRect = Rect(botRectOrigin, botRectSize);
    auto closeRect = Rect(botRectOrigin + botRectOffset, botRectSize);
    auto refreshRect = Rect(botRectOrigin + 2 * botRectOffset, botRectSize);

    // Calculate the dice bounding rect

    auto background = MenuItemImage::create("menu_bg.png", "menu_bg.png");

    if (background == nullptr || background->getContentSize().width <= 0 ||
        background->getContentSize().height <= 0) {
        problemLoading("'menu_bg.png'");
    } else {
        background->setPosition(origin.x, origin.y);
    }

    _menu = Menu::create(background, NULL);
    //_menu->setPosition(origin.x-600, origin.y+200);
    _menu->setPosition(240, 160);
    this->addChild(_menu, 1);
    _menuD.setMenu(_menu);
    //_menu.swipeDownwards();
    //
    auto keyListener = EventListenerKeyboard::create();
    keyListener->onKeyReleased = [=](EventKeyboard::KeyCode kc, Event *event) {
        if (kc == EventKeyboard::KeyCode::KEY_L) {
            _menuD.swipeDownwards();
        } else if (kc == EventKeyboard::KeyCode::KEY_K) {
            _menuD.swipeRight();
        } else if (kc == EventKeyboard::KeyCode::KEY_I) {
            _menuD.swipeLeft();
        } else if (kc == EventKeyboard::KeyCode::KEY_J) {
            _menuD.swipeUpwards();
        }
    };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);

    /*auto swipeEventListener = EventListenerTouchOneByOne::create();
    swipeEventListener->onTouchBegan =
        CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    swipeEventListener->onTouchEnded =
        CC_CALLBACK_2(HelloWorld::onTouchEnded, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(swipeEventListener,
                                                             this);*/
}

#ifdef COCOS2D_DEBUG
void HelloWorld::insertDebugLabels() {
    // create label atlas for showing raw accel data
    accelLabel = Label::create();
    accelLabel->retain();
    char buffer[30] = {0};
    sprintf(buffer, "A: %+1.3G / %+1.3G / %+1.3G", 0., 0., 0.);
    accelLabel->initWithTTF(buffer, "fonts/Marker Felt.ttf", 24);
    if (accelLabel == nullptr) {
        problemLoading("'fonts/Marker Felt.ttf'");
    } else {
        accelLabel->setPosition(Vec2(150, 100));
        this->addChild(accelLabel, 1);
    }

    // create label atlas for showing raw gravity data
    gravityLabel = Label::create();
    gravityLabel->retain();
    sprintf(buffer, "G: %+1.3G / %+1.3G / %+1.3G", 0., 0., 0.);
    gravityLabel->initWithTTF(buffer, "fonts/Marker Felt.ttf", 24);
    if (gravityLabel == nullptr) {
        problemLoading("'fonts/Marker Felt.ttf'");
    } else {
        gravityLabel->setPosition(Vec2(150, 150));
        this->addChild(gravityLabel, 1);
    }
}
#endif

void HelloWorld::initPhysicsAndCamera() {

    // construct PhysicsWorld
    auto world = this->getPhysics3DWorld();
#ifdef COCOS2D_DEBUG
    const GLubyte *glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
    printf("GLSL Version         : %s\n", glslVersion);
    if (!world->isDebugDrawEnabled())
        world->setDebugDrawEnable(true);
#endif

    world->setGravity(Vec3(0, 0, 0));

    Size size = Director::getInstance()->getWinSize();
    auto camera = Camera::createPerspective(30.0f, size.width / size.height,
                                            1.0f, 1000.0f);
    camera->setPosition3D(Vec3(5.0f, 0.0f, 0.0f));
    camera->lookAt(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
    camera->setCameraFlag(CameraFlag::USER1);
    this->addChild(camera);

    this->setPhysics3DDebugCamera(camera);
}

void HelloWorld::insertDice() {
    Physics3DRigidBodyDes rbDes;

    rbDes.originalTransform.rotateY(M_PI / 6);
    rbDes.mass = 1.f;
    rbDes.shape = Physics3DShape::createBox(Vec3(0.5f, 0.5f, 0.5f));
    rbDes.disableSleep = true;
    auto sprite = PhysicsSprite3D::create("box.c3t", &rbDes);
    auto material =
        Material::createWithFilename("materials/glossyGlass.material");
    sprite->setMaterial(material);

    if (sprite == nullptr) {
        problemLoading("'box.c3t'");
    } else {

        dice = static_cast<Physics3DRigidBody *>(sprite->getPhysicsObj());
        dice->setLinearFactor(Vec3::ONE);
        // dice->setAngularVelocity(Vec3(1, 1, 0));
        dice->setCcdMotionThreshold(0.1f);
        dice->setCcdSweptSphereRadius(0.1f);
        // add the sprite as a child to this layer
        this->addChild(sprite);

        sprite->setScale(0.5f);
        sprite->syncPhysicsToNode();
        sprite->setSyncFlag(
            Physics3DComponent::PhysicsSyncFlag::PHYSICS_TO_NODE); // sync node
                                                                   // to physics
        sprite->setCameraMask((unsigned short)CameraFlag::USER1);
    }
}

void HelloWorld::addQBox() {
    auto quads = getQuadPlanes();

    auto colCb = [](const Physics3DObject *ci) {
        AudioEngine::play2d("hitWall.mp3", false, 1.0);
    };

    auto floor = Sprite3D::create("flat.c3t");
    if (floor == nullptr) {
        problemLoading("'flat.c3t'");
    } else {
        // floor->setRotation3D(Vec3(90, 0, 0));
        floor->setPosition3D(Vec3(-15, 0, 0));
        floor->setScale(0.75f);
        this->addChild(floor);
        floor->setCameraMask((unsigned short)CameraFlag::USER1);
    }

    for (auto quad : quads) {
        Physics3DRigidBodyDes rbDes;
        Physics3DColliderDes colliderDes;

        rbDes.mass = 0.f;
        rbDes.shape = Physics3DShape::createConvexHull(quad.data(), 4);
        colliderDes.shape = rbDes.shape;
        colliderDes.isTrigger = true;
        auto plane = PhysicsSprite3D::create("plane.c3t", &rbDes);
        auto collider = Physics3DCollider::create(&colliderDes);
        auto component = Physics3DComponent::create(collider);
        auto node = Node::create();
        node->addComponent(component);
        node->setCameraMask((unsigned short)CameraFlag::USER1);

        this->addChild(node);

        collider->onTriggerEnter = colCb;
        if (plane == nullptr) {
            problemLoading("'plane.c3t'");
        } else {
            auto rigidBody =
                static_cast<Physics3DRigidBody *>(plane->getPhysicsObj());
            rigidBody->setLinearFactor(Vec3::ONE);
            rigidBody->setAngularVelocity(Vec3(0, 0, 0));
            rigidBody->setCcdMotionThreshold(0.5f);
            rigidBody->setCcdSweptSphereRadius(0.4f);

            this->addChild(plane);

            plane->syncNodeToPhysics();
            plane->setSyncFlag(Physics3DComponent::PhysicsSyncFlag::
                                   PHYSICS_TO_NODE); // sync node to
                                                     // physics
            plane->setCameraMask((unsigned short)CameraFlag::USER1);
        }
    }
}

void HelloWorld::createAccelerationCallbacks() {
    // create a matrix which aligns the accelerometer frame with the game frame
    // by experminents, it seems just one rotation is needed about Y
    Mat4::createRotationY(MATH_PIOVER2, &android2Game);

    // add the motion sensor
    Device::setAccelerometerEnabled(true);
    Device::setKeepScreenOn(true);

    auto accelListener = EventListenerAcceleration::create(
        CC_CALLBACK_2(HelloWorld::onAcceleration, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(accelListener,
                                                             this);

    auto gravListener = EventListenerCustom::create(
        "gravity", CC_CALLBACK_1(HelloWorld::onGravity, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(gravListener,
                                                             this);
}

std::vector<std::vector<cocos2d::Vec3>> HelloWorld::getQuadPlanes() {
    std::vector<std::vector<Vec3>> ret;

    int x_min = 1, x_max = -1;
    int y_min = -1, y_max = 1;
    int z_min = -2, z_max = 2;

    // bottom face
    std::vector<Vec3> bot;
    bot.push_back(Vec3(x_min, y_min, z_min));
    bot.push_back(Vec3(x_min, y_min, z_max));
    bot.push_back(Vec3(x_min, y_max, z_max));
    bot.push_back(Vec3(x_min, y_max, z_min));

    ret.push_back(bot);

    // top face
    std::vector<Vec3> top;
    top.push_back(Vec3(x_max, y_min, z_min));
    top.push_back(Vec3(x_max, y_min, z_max));
    top.push_back(Vec3(x_max, y_max, z_max));
    top.push_back(Vec3(x_max, y_max, z_min));

    ret.push_back(top);

    // left face
    std::vector<Vec3> left;
    left.push_back(Vec3(x_min, y_min, z_min));
    left.push_back(Vec3(x_min, y_max, z_min));
    left.push_back(Vec3(x_max, y_max, z_min));
    left.push_back(Vec3(x_max, y_min, z_min));

    ret.push_back(left);

    // right face
    std::vector<Vec3> right;
    right.push_back(Vec3(x_min, y_min, z_max));
    right.push_back(Vec3(x_min, y_max, z_max));
    right.push_back(Vec3(x_max, y_max, z_max));
    right.push_back(Vec3(x_max, y_min, z_max));

    ret.push_back(right);

    // down face
    std::vector<Vec3> down;
    down.push_back(Vec3(x_min, y_min, z_min));
    down.push_back(Vec3(x_min, y_min, z_max));
    down.push_back(Vec3(x_max, y_min, z_max));
    down.push_back(Vec3(x_max, y_min, z_min));

    ret.push_back(down);

    // up face
    std::vector<Vec3> up;
    up.push_back(Vec3(x_min, y_max, z_min));
    up.push_back(Vec3(x_min, y_max, z_max));
    up.push_back(Vec3(x_max, y_max, z_max));
    up.push_back(Vec3(x_max, y_max, z_min));

    ret.push_back(up);

    return ret;
}

void HelloWorld::menuCloseCallback(Ref *pSender) {
    // Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();
}

bool HelloWorld::onTouchBegan(Touch *touch, Event *event) {
    _touchBeginLoc = new Vec2(touch->getLocation());

    return true;
}

void HelloWorld::onTouchEnded(Touch *touch, Event *event) {
    if (!_touchBeginLoc)
        return;

    const auto start = *_touchBeginLoc;
    const auto end = touch->getLocation();
    event->stopPropagation();

    const auto dx = end.x - start.x;
    const auto dy = end.y - start.y;
    const auto length = sqrt(dx * dx + dy * dy);
    const auto verticalSwipe = abs(dy) > abs(dx);

    delete _touchBeginLoc;
    _touchBeginLoc = nullptr;

    if (length < _minSwipeLen) {
        return;
    }

    if (verticalSwipe && dy > 0) {
        // upward
        _menuD.swipeUpwards();
    } else if (verticalSwipe) {
        // downward
        _menuD.swipeDownwards();
    } else if (!verticalSwipe && dx > 0) {
        // right
        _menuD.swipeRight();
    } else {
        // left
        _menuD.swipeLeft();
    }
}

void HelloWorld::onAcceleration(Acceleration *accel, Event *event) {
    auto android_l_accel = android2Game * Vec3(accel->x, accel->y, accel->z);

#ifdef COCOS2D_DEBUG
    _accumlaDt += Director::getInstance()->getDeltaTime();
    if (_accumlaDt > CC_DIRECTOR_STATS_INTERVAL) {
        char buffer[30] = {0};
        sprintf(buffer, "A: %+1.3G / %+1.3G / %+1.3G", accel->x, accel->y,
                accel->z);
        accelLabel->setString(buffer);
        _accumlaDt = 0;
    }
#endif
    // apply a scaled force in the opposite direction to make it work
    // the impulses are too wild,
    // scale these down and also negate the world gravity while applying this
    // for more realistic behavior
    dice->applyCentralImpulse(android_l_accel * -2);
}

void HelloWorld::onGravity(EventCustom *event) {
    Acceleration *gravity = static_cast<Acceleration *>(event->getUserData());
    auto android_gravity =
        android2Game * Vec3(gravity->x, gravity->y, gravity->z);

#ifdef COCOS2D_DEBUG
    _accumgDt += Director::getInstance()->getDeltaTime();
    if (_accumgDt > CC_DIRECTOR_STATS_INTERVAL) {
        char buffer[30] = {0};
        sprintf(buffer, "G: %+1.3G / %+1.3G / %+1.3G", gravity->x, gravity->y,
                gravity->z);
        gravityLabel->setString(buffer);
        _accumgDt = 0;
    }
#endif

    CC_SAFE_DELETE(gravity);
    // Multiply this by 9.8 or some scale factor for better augmented reality
    this->getPhysics3DWorld()->setGravity(android_gravity * TG3_GRAVITY_EARTH);
}
