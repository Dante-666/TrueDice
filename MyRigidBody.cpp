#include "MyRigidBody.h"

NS_CC_BEGIN

int MyPhysicsSprite3D::idx = 0;
/* auto colCb = [](const Physics3DObject *ci) {
        auto ps =
        PUParticleSystem3D::create("Particle3D/scripts/mp_hit_04.pu");
        ps->setPosition3D(ci.collisionPointList[0].worldPositionOnB);
        ps->setScale(0.05f);
        ps->startParticleSystem();
        ps->setCameraMask(2);
        this->addChild(ps);
        ps->runAction(Sequence::create(
            DelayTime::create(1.0f),
            CallFunc::create([=]() { ps->removeFromParent(); }), nullptr));
    };
*/

void MyPhysicsSprite3D::pairCallBack(const Physics3DCollisionInfo &ci) {
    if (!ci.dynamicCollision) {
        // Expects objB to be the ground always
        auto mask = ci.objB->getMask();
        if ((mask & id) == 0) {
            if (this->getChildrenCount() < 6) {
                AudioEngine::play2d("sfx/hitWall.mp3", false, 1.0);
                auto ps = PUParticleSystem3D::create(
                    "particles/lightningBolt.pu",
                    //"particles/explosionSystem.pu",
                    "materials/pu_light_fire.material");
                ps->setPosition3D(ci.collisionPointList[0].worldPositionOnA);
                ps->setScale(0.2f);
                ps->startParticleSystem();
                ps->setCameraMask(2);
                this->addChild(ps);
                ps->runAction(Sequence::create(
                    DelayTime::create(3),
                    CallFunc::create([=]() { ps->removeFromParent(); }),
                    nullptr));
            }
            ci.objB->setMask(mask | id);
        }
    } else {
        auto mask = ci.objB->getMask();
        if ((mask & id) == 0) {
            if (this->getChildrenCount() < 6) {
                // could play different audio here
                AudioEngine::play2d("sfx/hitWall.mp3", false, 0.5);
                auto ps = PUParticleSystem3D::create(
                    "particles/lightningBolt.pu",
                    //"particles/explosionSystem.pu",
                    "materials/pu_light_fire.material");
                ps->setPosition3D(ci.collisionPointList[0].worldPositionOnA);
                ps->setScale(0.2f);
                ps->startParticleSystem();
                ps->setCameraMask(2);
                this->addChild(ps);
                ps->runAction(Sequence::create(
                    DelayTime::create(3),
                    CallFunc::create([=]() { ps->removeFromParent(); }),
                    nullptr));
            }
            ci.objB->setMask(mask | id);
        }
    }
}
/*if (ci.objA == this->getPhysicsObj()) {
    auto mask = ci.objB->getMask();
    if ((mask & id) == 0) {
        auto vel = static_cast<Physics3DRigidBody
*>(ci.objA)->getLinearVelocity(); AudioEngine::play2d("sfx/hitWall.mp3",
false, 1.0); auto ps =
            PUParticleSystem3D::create("particles/mp_hit_04.pu");
        ps->setPosition3D(ci.collisionPointList[0].worldPositionOnA);
        ps->setScale(0.05f);
        ps->startParticleSystem();
        ps->setCameraMask(2);
        this->addChild(ps);
        ps->runAction(Sequence::create(
            DelayTime::create(1.0f),
            CallFunc::create([=]() { ps->removeFromParent(); }),
nullptr));

        ci.objB->setMask(mask | id);
    }
} else if (ci.objB == this->getPhysicsObj()) {
    auto mask = ci.objA->getMask();
    if ((mask & id) == 0) {
        auto vel = static_cast<Physics3DRigidBody
*>(ci.objB)->getLinearVelocity(); AudioEngine::play2d("sfx/hitWall.mp3",
false, 1.0); auto ps =
            PUParticleSystem3D::create("particles/mp_hit_04.pu");
        ps->setPosition3D(ci.collisionPointList[0].worldPositionOnB);
        ps->setScale(0.05f);
        ps->startParticleSystem();
        ps->setCameraMask(2);
        this->addChild(ps);
        ps->runAction(Sequence::create(
            DelayTime::create(1.0f),
            CallFunc::create([=]() { ps->removeFromParent(); }),
nullptr)); ci.objA->setMask(mask | id);
    }
}*/

void MyPhysicsSprite3D::bindPairCallBack(Physics3DRigidBody *rb) {
    rb->setCollisionCallback(std::bind(&MyPhysicsSprite3D::pairCallBack, this,
                                       std::placeholders::_1));
}

void MyPhysicsSprite3D::exitCallBack(Physics3DObject *other) {
    other->setMask(other->getMask() & !id);
}

void MyPhysicsSprite3D::bindExitCallBack(Physics3DCollider *collider) {
    id = 1 << idx++;
    collider->onTriggerExit = std::bind(&MyPhysicsSprite3D::exitCallBack, this,
                                        std::placeholders::_1);
}

MyPhysicsSprite3D *MyPhysicsSprite3D::create(const Color4F &color) {
    Physics3DRigidBodyDes rbDes;
    Physics3DColliderDes colliderDes;

    rbDes.mass = 1.f;
    rbDes.shape = Physics3DShape::createBox({0.5f, 0.5f, 0.5f});
    rbDes.disableSleep = true;
    colliderDes.shape = Physics3DShape::createBox({0.501f, 0.501f, 0.501f});
    colliderDes.isTrigger = true;
    auto collider = Physics3DCollider::create(&colliderDes);
    collider->setCollisionFlags(4);
    auto component = Physics3DComponent::create(collider);

    // Add this to a dummy node since Physics3DRigidBody will already
    // have a RigidBody component
    auto node = Node::create();
    node->addComponent(component);
    node->setCameraMask((unsigned short)CameraFlag::USER1);

    auto sprite = new (std::nothrow) MyPhysicsSprite3D();

    sprite->bindExitCallBack(collider);

    if (sprite && sprite->initWithFile("models/rDice.obj")) {
        auto obj = Physics3DRigidBody::create(&rbDes);
        sprite->_physicsComponent =
            Physics3DComponent::create(obj, Vec3::ZERO, Quaternion::ZERO);
        sprite->addComponent(sprite->_physicsComponent);
        sprite->_contentSize = sprite->getBoundingBox().size;
        sprite->autorelease();

        auto material =
            Material::createWithFilename("materials/colorDice.material");
        auto ps =
            material->getTechnique()->getPassByIndex(0)->getProgramState();
        auto loc = ps->getUniformLocation("dice_color");

        ps->setUniform(loc, &color, sizeof(Color4F));
        sprite->setMaterial(material);

        auto dice = static_cast<Physics3DRigidBody *>(sprite->getPhysicsObj());
        dice->setLinearFactor(Vec3::ONE);
        dice->setAngularVelocity(Vec3(1, 1, 0));
        dice->setCcdMotionThreshold(0.1f);
        dice->setCcdSweptSphereRadius(0.1f);
        dice->setMask(0);
        sprite->bindPairCallBack(dice);

        // add the trigger to this 3D object
        sprite->addChild(node);

        sprite->setScale(.25f);
        sprite->syncPhysicsToNode();
        sprite->setSyncFlag(
            Physics3DComponent::PhysicsSyncFlag::PHYSICS_TO_NODE); // sync node
                                                                   // to physics
        sprite->setCameraMask((unsigned short)CameraFlag::USER1);
    }

    return sprite;
};

NS_CC_END
