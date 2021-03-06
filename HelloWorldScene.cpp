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
#include <cmath>
#include <iostream>
#include <new>
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

    Director::getInstance()->setDisplayStats(false);

    insertMainMenu();

#ifdef COCOS2D_DEBUG
    insertDebugLabels();
#endif

    initPhysicsAndCamera();
    // generic idea would be to take the camera furstum and intersect it with
    // two planes and figure out the extremities of the box and calculate the
    // actual box
    addQBox();

    insertDice(_diceUI->getColor());

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
    auto h4_5 = halfRect.size.height * 4 / 5;
    auto dieRect = Rect(orig, Size(botRectOrigin.x, h4_5));

    // Paricle effect
    auto pEffSize = Size(dieRect.size.width / 2,
                         (halfRect.size.height - dieRect.size.height));
    auto pEffOrigin = Vec2(0, dieRect.size.height);
    auto pEffOffset = Vec2(pEffSize.width, 0);
    auto fireRect = Rect(pEffOrigin, pEffSize);
    auto lightRect = Rect(pEffOrigin + pEffOffset, pEffSize);

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
    creditSize.width /= 16;
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
    auto copySize = leftMainSize;
    copySize.width /=
        8; // leftMainSize.width - (iconRect.origin.x + iconSize.width);
    auto copyRect = Rect(iconRect.origin + Vec2(iconSize.width, 0), copySize);

#ifdef COCOS2D_DEBUG
    printSize(leftMainSize);
    printRect(inkRect);
    printRect(copyRect);
#endif

    TTFConfig labelConfig;
    labelConfig.fontFilePath = "fonts/Marker Felt.ttf";
    labelConfig.fontSize = 36;
    labelConfig.glyphs = GlyphCollection::DYNAMIC;
    labelConfig.outlineSize = 0;
    labelConfig.customGlyphs = nullptr;
    labelConfig.distanceFieldEnabled = false;

    TTFConfig bigLabelConfig = labelConfig;
    bigLabelConfig.fontSize = 56;

    auto createLabel = [](TTFConfig &labelConfig, const std::string &text,
                          TextHAlignment align =
                              TextHAlignment::LEFT) -> Label * {
        Color4B textCol(21, 27, 31, 255);
        auto ret = Label::createWithTTF(labelConfig, text, align);
        ret->setTextColor(textCol);
        ret->enableShadow(Color4B::BLACK, Size(2.5, -2.5), 8);
        return ret;
    };

    auto usageLabel = createLabel(bigLabelConfig, "Usage");
    auto instLabel = createLabel(
        labelConfig,
        "Accelerometer is usually offset from the center and "
        "close to where the camera is.\n Holding the mobile as depicted and "
        "shaking it left and right should result in the best effect.",
        TextHAlignment::LEFT);
    instLabel->setMaxLineWidth(vSize.height * 0.8);
    auto creditLabel =
        createLabel(bigLabelConfig, "Credits", TextHAlignment::CENTER);
    auto infoLabel =
        createLabel(labelConfig,
                    "Artwork : Siddharth\n"
                    "Design : Siddharth\n"
                    "Testing : Siddharth\n\n"
                    "SFX @ zapsplat.com\n"
                    "Textures @ FantasyStock, 10ravens@DeviantArt\n"
                    "Contact : SiddharthJSingh@protonmail.com",
                    TextHAlignment::CENTER);
    auto builtLabel =
        createLabel(labelConfig, "Built With", TextHAlignment::CENTER);
    auto copyLabel = createLabel(labelConfig,
                                 "Copyright 2021, Siddharth J Singh\n"
                                 "All rights reserved",
                                 TextHAlignment::CENTER);

    auto loadMenuItem =
        [](const std::string &loc, Rect &pos, bool enabled = true,
           const std::string &sLoc = std::string()) -> MenuItemZoomImage * {
        auto imgLoc = "menuIcons/" + loc;
        auto sImgLoc = sLoc.size() > 0 ? "menuIcons/" + sLoc : sLoc;

        auto mii = MenuItemZoomImage::create(imgLoc, sImgLoc);

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

    auto loadMenuLabel = [](Label *label, const Rect &pos) -> MenuItem * {
        auto mli = MenuItemLabel::create(label);
        mli->setRotation(-90);
        mli->setPosition(pos.getMidX(), pos.getMidY());

        return mli;
    };

    // This is for dice object only
    auto loadMenuItemMulti = [](const std::string &loc,
                                const Rect &pos) -> MenuItemMultiImage * {
        auto mim = MenuItemMultiImage::create("menuIcons/" + loc + "G.png");
        mim->insertImage("menuIcons/" + loc + "B.png");
        mim->insertImage("menuIcons/" + loc + "R.png");

        mim->setRotation(-90);

        if (mim == nullptr || mim->getContentSize().width <= 0 ||
            mim->getContentSize().height <= 0) {
            problemLoading(loc.c_str());
        } else {
            mim->setPosition(pos.getMidX(), pos.getMidY());
            mim->setEnabled(true);
        }

        return mim;
    };

    // Parchment background should be 3xhalfRects
    auto threeHalfRects = halfRect;
    threeHalfRects.size.height =
        3 * halfRect.size.height + 2 * halfRect.origin.y;
    threeHalfRects.origin.y = -halfRect.origin.y - halfRect.size.height;

    using cocos2d::backend::SamplerAddressMode;
    using cocos2d::backend::SamplerFilter;
    auto texParams = Texture2D::TexParams{
        SamplerFilter::LINEAR, SamplerFilter::LINEAR,
        SamplerAddressMode::REPEAT, SamplerAddressMode::REPEAT};
    auto menuBGSprite = Sprite::create("menuIcons/parchment_seamless.jpg");
    if (menuBGSprite == nullptr || menuBGSprite->getContentSize().width < 0 ||
        menuBGSprite->getContentSize().height <= 0) {
        problemLoading("'parchment_seamless.jpg'");
    }

    menuBGSprite->getTexture()->setTexParameters(texParams);
    menuBGSprite->setTextureRect(threeHalfRects);

    auto bottomHalfRect = threeHalfRects;
    bottomHalfRect.size.width = 0.05 * threeHalfRects.size.width;
    Vec2 tempOrigin = bottomHalfRect.origin;
    bottomHalfRect.origin = Vec2(0, 0);

    auto menuBGBotSprite = Sprite::create("menuIcons/parchment_burnt.png");
    if (menuBGBotSprite == nullptr ||
        menuBGBotSprite->getContentSize().width < 0 ||
        menuBGBotSprite->getContentSize().height <= 0) {
        problemLoading("'parchment_burnt.png'");
    }

    auto vertShaderSrc =
        FileUtils::getInstance()->getStringFromFile("materials/burnt.vert");
    auto fragShaderSrc =
        FileUtils::getInstance()->getStringFromFile("materials/burnt.frag");

    auto *program = backend::Device::getInstance()->newProgram(vertShaderSrc,
                                                               fragShaderSrc);
    auto programState = new (std::nothrow) backend::ProgramState(program);
    auto scaleLoc = programState->getUniformLocation("scale");
    float scale = 1;
    programState->setUniform(scaleLoc, &scale, sizeof(float));
    menuBGBotSprite->setProgramState(programState);

    menuBGBotSprite->setTextureRect(bottomHalfRect);

    bottomHalfRect.origin = tempOrigin;
    bottomHalfRect.origin.x =
        halfRect.size.width -
        (menuBGBotSprite->getTexture()->getPixelsWide() * 8) / 11.0;

    auto menuBG = MenuItemSprite::create(menuBGSprite, menuBGSprite);
    menuBG->setPosition(threeHalfRects.getMidX(), threeHalfRects.getMidY());
    menuBG->setEnabled(false);

    auto menuBGBot = MenuItemSprite::create(menuBGBotSprite, menuBGBotSprite);
    menuBGBot->setPosition(bottomHalfRect.getMidX(), bottomHalfRect.getMidY());
    menuBGBot->setEnabled(false);

    // Center items
    auto close = loadMenuItem("close.png", closeRect);
    close->setCallback(CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    auto add = loadMenuItem("add.png", addRect);
    auto refresh = loadMenuItem("refresh.png", refreshRect);

    _diceUI = loadMenuItemMulti("dice", dieRect);
    _diceUI->setCallback(
        CC_CALLBACK_0(MenuItemMultiImage::activateNextImage, _diceUI));
    add->setCallback(
        std::bind(&HelloWorld::insertDice, this,
                  std::bind(&MenuItemMultiImage::getColor, _diceUI)));
    refresh->setCallback(std::bind(&HelloWorld::removeActiveDice, this));

    auto fire = loadMenuItem("fire.png", fireRect, true, "sfire.png");
    auto light = loadMenuItem("light.png", lightRect, true, "slight.png");
    _particleUI.push_back(fire);
    _particleUI.push_back(light);
    fire->setCallback(std::bind(&HelloWorld::selectParticleEffect, this,
                                std::placeholders::_1));
    light->setCallback(std::bind(&HelloWorld::selectParticleEffect, this,
                                 std::placeholders::_1));

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

    _menu = Menu::create(menuBG, menuBGBot, add, close, refresh, _diceUI, fire,
                         light, mobile, usage, inst, credit, info, built, ink,
                         coco, blender, copy, NULL);
    _menu->setPosition(vOrig.x - halfRect.size.width, vOrig.y);
    this->addChild(_menu, 1);
    _menuD.setNodeAndSize(_menu, halfRect);

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
    //
    char buffer[30] = {0};
    sprintf(buffer, "A: %+1.3G / %+1.3G / %+1.3G", 0., 0., 0.);
    accelLabel = Label::createWithTTF(buffer, "fonts/Marker Felt.ttf", 24);
    if (accelLabel == nullptr) {
        problemLoading("'fonts/Marker Felt.ttf'");
    } else {
        accelLabel->setPosition(Vec2(150, 100));
        this->addChild(accelLabel, 1);
    }

    // create label atlas for showing raw gravity data
    sprintf(buffer, "G: %+1.3G / %+1.3G / %+1.3G", 0., 0., 0.);
    gravityLabel = Label::createWithTTF(buffer, "fonts/Marker Felt.ttf", 24);
    if (gravityLabel == nullptr) {
        problemLoading("'fonts/Marker Felt.ttf'");
    } else {
        gravityLabel->setPosition(Vec2(150, 150));
        this->addChild(gravityLabel, 1);
    }
}

void HelloWorld::drawUniformData(cocos2d::backend::ProgramState *ps) {
    using cocos2d::backend::ShaderStage;
    auto uniMap =
        ps->getProgram()->getAllActiveUniformInfo(ShaderStage::VERTEX);
    auto ind = 0;
    for (auto it = uniMap.begin(); it != uniMap.end(); ++it) {
        auto scale = it->second;
        auto uniLabel = Label::create();
        uniLabel->retain();
        char buffer[80] = {0};
        sprintf(buffer, "%s : L: %d + O: %d", it->first.c_str(), scale.location,
                scale.bufferOffset);
        uniLabel->initWithTTF(buffer, "fonts/Marker Felt.ttf", 24);
        uniLabel->setColor(Color3B(128, 128, 128));
        if (uniLabel == nullptr) {
            problemLoading("'fonts/Marker Felt.ttf'");
        } else {
            uniLabel->setPosition(Vec2(1200, 900 - ind++ * 100));
            this->addChild(uniLabel, 1);
        }
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

    world->setGravity({0, 0, -10});

    Size size = Director::getInstance()->getVisibleSize();
    _camera =
        Camera::createPerspective(30.0f, size.width / size.height, 1.0f, 50.0f);
    _camera->setPosition3D(Vec3(5.0f, .0f, .0f));
    _camera->lookAt(Vec3::ZERO);
    _camera->setCameraFlag(CameraFlag::USER1);
    this->addChild(_camera);

    this->setPhysics3DDebugCamera(_camera);
}

void HelloWorld::insertDice(const Color4F &color) {

    if (_dices.size() >= 10) {
        return;
    }

    auto dice = MyPhysicsSprite3D::create(color);
    this->addChild(dice);
    if (_particleIndex == 0) {
        dice->conditionalCallBack(ParticleType::FIRE);
    } else if (_particleIndex == 1) {
        dice->conditionalCallBack(ParticleType::LIGHTNING);
    } else {
        dice->conditionalCallBack(ParticleType::NONE);
    }
    _dices.push_back(dice);
}

void HelloWorld::removeActiveDice() {
    while (_dices.size() > 1) {
        auto it = _dices.begin();
        this->removeChild(*it);
        _dices.erase(it);
    }
}

void HelloWorld::addQBox() {
    auto quads = getQuadPlanes();

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

        auto plane = new (std::nothrow) PhysicsSprite3D();
        if (plane == nullptr) {
            printf("Error while creating PhysicsSprite3D\n");
        } else {
            auto rigidBody = Physics3DRigidBody::create(&rbDes);
            rigidBody->setLinearFactor(Vec3::ONE);
            rigidBody->setAngularVelocity(Vec3(0, 0, 0));
            rigidBody->setCcdMotionThreshold(0.5f);
            rigidBody->setCcdSweptSphereRadius(0.4f);
            rigidBody->setMask(0);
            auto rbComponent = Physics3DComponent::create(rigidBody);
            plane->addComponent(rbComponent);
            plane->setContentSize(plane->getBoundingBox().size);
            plane->autorelease();

            this->addChild(plane);

            rbComponent->syncNodeToPhysics();
            rbComponent->setSyncFlag(Physics3DComponent::PhysicsSyncFlag::
                                         PHYSICS_TO_NODE); // sync node to
                                                           // physics
            plane->setCameraMask((unsigned short)CameraFlag::USER1);
        }
    }
}

void HelloWorld::createAccelerationCallbacks() {
    // create a matrix which aligns the accelerometer frame with the game frame
    // by experminents, it seems just one rotation is needed about Y
    Mat4::createRotationY(MATH_PIOVER2, &_android2Game);

    // add the motion sensor
    Device::setAccelerometerEnabled(true);
    // Turn screen off for power saving, user can anyhow control this
    Device::setKeepScreenOn(false);

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

    auto vSize = Director::getInstance()->getVisibleSize();
    auto vOrig = Director::getInstance()->getVisibleOrigin();

    // add methods for plane intersection
    auto corners = obtainIntersectionPoints(_camera);

    // bottom face
    std::vector<Vec3> bot;
    bot.push_back(corners[0]);
    bot.push_back(corners[1]);
    bot.push_back(corners[3]);
    bot.push_back(corners[2]);

    ret.push_back(bot);

    // top face
    std::vector<Vec3> top;
    top.push_back(corners[4]);
    top.push_back(corners[5]);
    top.push_back(corners[7]);
    top.push_back(corners[6]);

    ret.push_back(top);

    // left face
    std::vector<Vec3> left;
    left.push_back(corners[0]);
    left.push_back(corners[1]);
    left.push_back(corners[5]);
    left.push_back(corners[4]);

    ret.push_back(left);

    // right face
    std::vector<Vec3> right;
    right.push_back(corners[2]);
    right.push_back(corners[3]);
    right.push_back(corners[7]);
    right.push_back(corners[6]);

    ret.push_back(right);

    // down face
    std::vector<Vec3> down;
    down.push_back(corners[1]);
    down.push_back(corners[3]);
    down.push_back(corners[7]);
    down.push_back(corners[5]);

    ret.push_back(down);

    // up face
    std::vector<Vec3> up;
    up.push_back(corners[0]);
    up.push_back(corners[2]);
    up.push_back(corners[6]);
    up.push_back(corners[4]);

    ret.push_back(up);

    return ret;
}

void HelloWorld::selectParticleEffect(Ref *item) {
    auto img = static_cast<MenuItemZoomImage *>(item);
    if (img->isSelected()) {
        _particleIndex = -1;
        img->unSelectImage();
        for (int i = 0; i < _dices.size(); i++) {
            _dices[i]->conditionalCallBack(ParticleType::NONE);
        }
    } else {
        for (int i = 0; i < _particleUI.size(); i++) {
            if (img == _particleUI[i]) {
                _particleIndex = i;
            }
            _particleUI[i]->unSelectImage();
        }
        if (_particleIndex == 0) {
            for (int i = 0; i < _dices.size(); i++) {
                _dices[i]->conditionalCallBack(ParticleType::FIRE);
            }
        } else if (_particleIndex == 1) {
            for (int i = 0; i < _dices.size(); i++) {
                _dices[i]->conditionalCallBack(ParticleType::LIGHTNING);
            }
        }
        img->selectImage();
    }
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
    auto android_l_accel = _android2Game * Vec3(accel->x, accel->y, accel->z);

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

    auto gravity = this->getPhysics3DWorld()->getGravity();
    auto lDotg = gravity.dot(android_l_accel);
    auto cosine = lDotg / (gravity.length() * android_l_accel.length());
    auto alpha = -2.5;

    // This component is to smooth out the linear acceleration
    static float beta = 1/(LA_MAX*LA_MAX);
    auto normalized = android_l_accel.getNormalized();
    auto mag = android_l_accel.length();
    if (mag > LA_MAX) {
        android_l_accel = normalized * LA_MAX;
    } else {
        auto scale = beta * pow(mag, 3);
        android_l_accel = normalized * scale;
    }

    // apply a scaled force in the opposite direction to make it work
    // the impulses are too wild,
    // scale these down and also negate the world gravity while applying this
    // for more realistic behavior
    for (auto dice : _dices) {
        auto rb = static_cast<Physics3DRigidBody *>(dice->getPhysicsObj());
        rb->applyCentralImpulse(android_l_accel * alpha);
    }
}

void HelloWorld::onGravity(EventCustom *event) {
    Acceleration *gravity = static_cast<Acceleration *>(event->getUserData());
    auto android_gravity =
        _android2Game * Vec3(gravity->x, gravity->y, gravity->z);

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
