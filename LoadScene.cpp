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

#include "LoadScene.h"
#include <iostream>

USING_NS_CC;

Scene *LoadScreen::createScene() { return LoadScreen::create(); }

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char *filename) {
    printf("Error while loading: %s\n", filename);
    printf(
        "Depending on how you compiled you might have to add 'Resources/' in "
        "front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool LoadScreen::init() {

    if (!Scene::init()) {
        return false;
    }

    initLogo();

    return true;
}

void LoadScreen::initLogo() {
    auto vSize = Director::getInstance()->getVisibleSize();
    auto vOrig = Director::getInstance()->getVisibleOrigin();
    auto vRect = Rect(vOrig, vSize);

    std::string logoLoc = "menuIcons/be_studios_logo.png";

    auto be_studio_logos = MenuItemImage::create(logoLoc, logoLoc);
    be_studio_logos->setPosition(vRect.getMidX(), vRect.getMidY());
    be_studio_logos->setRotation(-90);
    be_studio_logos->setOpacity(0);

    this->addChild(be_studio_logos);

    ActionInterval *fadeIn = FadeIn::create(2.5f);
    ActionInterval *fadeOut = FadeOut::create(2.5f);

    ActionInstant *popScene =
        CallFunc::create([=]() { Director::getInstance()->popScene(); });

    Action *sequence = Sequence::create(fadeIn, fadeOut, popScene, nullptr);
    AudioEngine::play2d("sfx/loadScreen.mp3", false, 0.75);
    be_studio_logos->runAction(sequence);
}
