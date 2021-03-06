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
#include <iostream>
#include <vector>

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

    insertMainMenu();

#ifdef COCOS2D_DEBUG
    insertDebugLabels();
#endif

    initPhysicsAndCamera();
    // generic idea would be to take the camera furstum and intersect it with
    // two planes and figure out the extremities of the box and calculate the
    // actual box
    addQBox();

    insertDice();

    createAccelerationCallbacks();

    return true;
}

void HelloWorld::insertMainMenu() {
    auto vSize = Director::getInstance()->getVisibleSize();
    auto vOrig = Director::getInstance()->getVisibleOrigin();

#ifdef COCOS2D_DEBUG
    auto printVec = [](Vec2 &vec) {
        std::cout << "Vector : (" << vec.x << ", " << vec.y << ")" << std::endl;
    };
    auto printSize = [](Size &sz) {
        std::cout << "Size : (" << sz.width << ", " << sz.height << ")"
                  << std::endl;
    };
    auto printRect = [](Rect &rt) {
        std::cout << "Rect : (" << rt.origin.x << ", " << rt.origin.y << " | "
                  << rt.size.width << ", " << rt.size.height << ")"
                  << std::endl;
    };
#endif

    // This gives how much screen area we have to work with
    auto halfRect = Rect(vOrig, vSize);
    halfRect.size.width /= 2;
    auto orig = Vec2(0, 0);

    // Calculate the bounding rects of add, close and refresh buttons
    auto w1_3 = halfRect.size.height / 3;
    auto botRectOrigin = Vec2(halfRect.size.width - w1_3, 0);
    auto botRectOffset = Vec2(0, w1_3);
    auto botRectSize = Size(w1_3, w1_3);

    auto closeRect = Rect(botRectOrigin, botRectSize);
    auto addRect = Rect(botRectOrigin + botRectOffset, botRectSize);
    auto refreshRect = Rect(botRectOrigin + 2 * botRectOffset, botRectSize);

    // Calculate the dice bounding rect
    auto w3_5 = halfRect.size.height * 3 / 5;
    auto dieRect = Rect(orig, Size(botRectOrigin.x, w3_5));

    // Paricle effect
    auto pEffSize = Size(dieRect.size.width / 3,
                         (halfRect.size.height - dieRect.size.height) / 2);
    auto pEffOrigin =
        Vec2(dieRect.size.width - pEffSize.width, dieRect.size.height);
    auto pEffOffset = Vec2(0, pEffSize.height);
    auto fireRect = Rect(pEffOrigin, pEffSize);
    auto lightRect = Rect(pEffOrigin + pEffOffset, pEffSize);

    // Likewise for the colorChange icon
    auto colChangeSize = Size(dieRect.size.width - pEffSize.width,
                              halfRect.size.height - dieRect.size.height);
    auto colChangeRect = Rect(orig + Vec2(0, w3_5), colChangeSize);

    // Mobile Rect
    auto mobileSize = halfRect.size;
    mobileSize.width *= 2. / 3;
    auto mobileRect = Rect(orig + Vec2(0, halfRect.size.height), mobileSize);

    // Usage Rect
    auto usageSize = halfRect.size;
    usageSize.width /= 9;
    auto usageRect =
        Rect(mobileRect.origin + Vec2(mobileSize.width, 0), usageSize);

    // Instruction Rect
    auto instSize = halfRect.size;
    instSize.width *= 2. / 9;
    auto instRect =
        Rect(usageRect.origin + Vec2(usageRect.size.width, 0), instSize);

    // Left main Rect
    auto leftMainSize = halfRect.size;
    leftMainSize.width *= 4. / 5;
    leftMainSize.height *= 3. / 5;
    auto offset = Vec2(halfRect.size.width / 10, halfRect.size.height / 5);
    auto leftMainRect =
        Rect(orig - Vec2(0, halfRect.size.height) + offset, leftMainSize);

    // Credit Rect
    auto creditSize = leftMainSize;
    creditSize.width /= 8;
    auto creditRect = Rect(leftMainRect.origin, creditSize);

    // Info Rect
    auto infoSize = leftMainSize;
    infoSize.width *= 49. / 96;
    auto infoRect =
        Rect(creditRect.origin + Vec2(creditSize.width, 0), infoSize);

    // Builtby Rect also use this for copyRight
    auto builtSize = leftMainSize;
    builtSize.width /= 12;
    auto builtRect = Rect(infoRect.origin + Vec2(infoSize.width, 0), builtSize);

    // Icon Rects
    auto iconSize = leftMainSize;
    iconSize.width *= 9. / 32;
    iconSize.height = leftMainSize.height / 3;
    auto iconRect = Rect(builtRect.origin + Vec2(builtSize.width, 0), iconSize);
    auto iconOffSet = Vec2(0, iconSize.height);
    auto inkRect = Rect(iconRect.origin, iconRect.size);
    auto cocoRect = Rect(iconRect.origin + iconOffSet, iconRect.size);
    auto blenderRect = Rect(iconRect.origin + 2 * iconOffSet, iconRect.size);

    // Copyright Rect
    auto copyRect =
        Rect(Vec2(halfRect.size.width - builtSize.width, iconRect.origin.y),
             builtSize);

    TTFConfig labelConfig;
    labelConfig.fontFilePath = "fonts/Marker Felt.ttf";
    labelConfig.fontSize = 24;
    labelConfig.glyphs = GlyphCollection::DYNAMIC;
    labelConfig.outlineSize = 0;
    labelConfig.customGlyphs = nullptr;
    labelConfig.distanceFieldEnabled = false;

    TTFConfig bigLabelConfig = labelConfig;
    bigLabelConfig.fontSize = 36;

    auto usageLabel = Label::createWithTTF(bigLabelConfig, "Usage");
    auto instLabel = Label::createWithTTF(
        labelConfig,
        "Accelerometer is usually offset from the center and "
        "close to where the camera is.\n Holding the mobile as depicted and "
        "shaking it left and right should result in the best effect.",
        TextHAlignment::LEFT);
    auto creditLabel =
        Label::createWithTTF(bigLabelConfig, "Credits", TextHAlignment::CENTER);
    auto infoLabel =
        Label::createWithTTF(labelConfig,
                             "Artwork : Siddharth\n"
                             "Design : Siddharth\n\n"
                             "SFX @ <source>\n"
                             "Textures @ FantasyStock|10ravens@DeviantArt\n"
                             "Contact : SiddharthJSingh@protonmail.com",
                             TextHAlignment::CENTER);
    auto builtLabel =
        Label::createWithTTF(labelConfig, "Built With", TextHAlignment::CENTER);
    auto copyLabel = Label::createWithTTF(labelConfig,
                                          "Copyright 2021, Siddharth J Singh\n"
                                          "All rights reserved",
                                          TextHAlignment::CENTER);

    auto loadMenuItem = [](std::string loc, Rect &pos,
                           bool enabled = true) -> MenuItem * {
        auto imgLoc = "menuIcons/" + loc;
        auto mii = MenuItemZoomImage::create(imgLoc);
        mii->setRotation(-90);

        if (mii == nullptr || mii->getContentSize().width <= 0 ||
            mii->getContentSize().height <= 0) {
            problemLoading(loc.c_str());
        } else {
            mii->setPosition(pos.getMidX(), pos.getMidY());
            mii->setEnabled(enabled);
        }

        return mii;
    };

    auto loadMenuLabel = [](Label *label, Rect &pos) -> MenuItem * {
        auto mli = MenuItemLabel::create(label);
        mli->setRotation(-90);
        mli->setPosition(pos.getMidX(), pos.getMidY());

        return mli;
    };

    // Parchment background should be 3xhalfRects
    auto threeHalfRects = halfRect;
    threeHalfRects.size.height = 3 * halfRect.size.height;
    threeHalfRects.origin.y = halfRect.origin.y - halfRect.size.height;

    using cocos2d::backend::SamplerAddressMode;
    using cocos2d::backend::SamplerFilter;
    auto texParams = Texture2D::TexParams{
        SamplerFilter::LINEAR, SamplerFilter::LINEAR,
        SamplerAddressMode::REPEAT, SamplerAddressMode::REPEAT};
    auto menuBGSprite = Sprite::create("menuIcons/parchment_seamless.jpg");
    if (menuBGSprite == nullptr || menuBGSprite->getContentSize().width < 0 ||
        menuBGSprite->getContentSize().height <= 0) {
        problemLoading("'parchment_sealmess.jpg'");
    }

    menuBGSprite->getTexture()->setTexParameters(texParams);
    menuBGSprite->setTextureRect(threeHalfRects);

    auto menuBG = MenuItemSprite::create(menuBGSprite, menuBGSprite);
    menuBG->setPosition(threeHalfRects.getMidX(), threeHalfRects.getMidY());
    menuBG->setEnabled(false);

    // Center items
    auto close = loadMenuItem("close.png", closeRect);
    close->setCallback(CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    auto add = loadMenuItem("add.png", addRect);
    auto refresh = loadMenuItem("refresh.png", refreshRect);
    auto dice = loadMenuItem("dice.png", dieRect, false);
    auto colChange = loadMenuItem("colChange.png", colChangeRect);
    auto fire = loadMenuItem("fire.png", fireRect);
    auto light = loadMenuItem("light.png", lightRect);

    // Right items
    auto mobile = loadMenuItem("mobile.png", mobileRect, false);
    auto usage = loadMenuLabel(usageLabel, usageRect);
    auto inst = loadMenuLabel(instLabel, instRect);

    // Left items
    auto credit = loadMenuLabel(creditLabel, creditRect);
    auto info = loadMenuLabel(infoLabel, infoRect);
    auto built = loadMenuLabel(builtLabel, builtRect);
    auto ink = loadMenuItem("inkscape.png", inkRect, false);
    auto coco = loadMenuItem("cocos.png", cocoRect, false);
    auto blender = loadMenuItem("blender.png", blenderRect, false);
    auto copy = loadMenuLabel(copyLabel, copyRect);

    //_menu = Menu::create(add, close, refresh, dice, colChange, fire,
    _menu = Menu::create(menuBG, add, close, refresh, dice, colChange, fire,
                         light, mobile, usage, inst, credit, info, built, ink,
                         coco, blender, copy, NULL);
    _menu->setPosition(vOrig.x - halfRect.size.width, vOrig.y);
    //_menu->setPosition(vOrig.x , vOrig.y);
    this->addChild(_menu, 1);
    _menuD.setNodeAndSize(_menu, halfRect);

#ifdef COCOS2D_DEBUG
    auto showRect = dieRect;
    auto rty = Label::create();
    rty->retain();
    char buffer[40] = {0};
    sprintf(buffer, "Rect : (%1.2f, %1.2f) , (%1.2f, %1.2f)", showRect.origin.x,
            showRect.origin.y, showRect.size.width, showRect.size.height);
    rty->initWithTTF(buffer, "fonts/Marker Felt.ttf", 24);
    if (rty == nullptr) {
        problemLoading("'fonts/Marker Felt.ttf'");
    } else {
        rty->setPosition(Vec2(1200, 100));
        this->addChild(rty, 1);
    }

    printRect(closeRect);
    Rect cRect = close->rect();
    printRect(cRect);
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
    auto keyListener = EventListenerKeyboard::create();
    keyListener->onKeyReleased = [=](EventKeyboard::KeyCode kc, Event *event) {
        if (kc == EventKeyboard::KeyCode::KEY_L) {
            _menuD.swipeDownwards();
        } else if (kc == EventKeyboard::KeyCode::KEY_I) {
            _menuD.swipeRight();
        } else if (kc == EventKeyboard::KeyCode::KEY_K) {
            _menuD.swipeLeft();
        } else if (kc == EventKeyboard::KeyCode::KEY_J) {
            _menuD.swipeUpwards();
        }
    };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);
#else
    auto swipeEventListener = EventListenerTouchOneByOne::create();
    swipeEventListener->onTouchBegan =
        CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    swipeEventListener->onTouchEnded =
        CC_CALLBACK_2(HelloWorld::onTouchEnded, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(swipeEventListener,
                                                             this);
#endif
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
    auto sprite = PhysicsSprite3D::create("models/box.c3t", &rbDes);

    if (sprite == nullptr) {
        problemLoading("'models/box.c3t'");
    } else {
        auto material =
            Material::createWithFilename("materials/glossyGlass.material");
        sprite->setMaterial(material);
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
        AudioEngine::play2d("sfx/hitWall.mp3", false, 1.0);
    };

    auto floor = Sprite3D::create("models/flat.c3t");
    if (floor == nullptr) {
        problemLoading("'models/flat.c3t'");
    } else {
        auto material =
            Material::createWithFilename("materials/repeat.material");
        auto ps =
            material->getTechnique()->getPassByIndex(0)->getProgramState();
        auto scale = ps->getUniformLocation("scale");
        float uScale{3};
        ps->setUniform(scale, &uScale, sizeof(float));
        floor->setMaterial(material);

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
        auto plane = PhysicsSprite3D::create("models/box.c3t", &rbDes);
        auto collider = Physics3DCollider::create(&colliderDes);
        auto component = Physics3DComponent::create(collider);
        auto node = Node::create();
        node->addComponent(component);
        node->setCameraMask((unsigned short)CameraFlag::USER1);

        this->addChild(node);

        collider->onTriggerEnter = colCb;
        if (plane == nullptr) {
            problemLoading("'models/box.c3t'");
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
    // std::cout<<"clicked"<<std::endl;
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
    const auto verticalSwipe = abs(dx) > abs(dy);

    delete _touchBeginLoc;
    _touchBeginLoc = nullptr;

    if (length < _minSwipeLen) {
        return;
    }

    if (verticalSwipe && dy < 0) {
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
