#include "2d/CCCamera.h"
#include "2d/CCActionInterval.h"
#include "2d/CCActionInstant.h"
#include "physics3d/CCPhysics3DShape.h"
#include "physics3d/CCPhysics3DObject.h"
#include "physics3d/CCPhysicsSprite3D.h"
#include "extensions/Particle3D/PU/CCPUParticleSystem3D.h"
#include "renderer/CCMaterial.h"
#include "renderer/CCPass.h"
#include "audio/include/AudioEngine.h"

NS_CC_BEGIN

enum class ParticleType {
    NONE, FIRE, LIGHTNING
};

class MyPhysicsSprite3D : public PhysicsSprite3D {
    static int _idx;

    int _id;

    ParticleType _type = ParticleType::NONE;

  public:
    static MyPhysicsSprite3D *create(const Color4F &color);

    void bindPairCallBack(Physics3DRigidBody* collider);
    void pairCallBack(const Physics3DCollisionInfo& ci);

    void bindExitCallBack(Physics3DCollider* collider);
    void exitCallBack(Physics3DObject *other);

    void conditionalCallBack(const ParticleType& type);
};

NS_CC_END
