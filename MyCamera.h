#include "2d/CCCamera.h"
#include "3d/CCFrustum.h"

NS_CC_BEGIN

class CC_DLL MyCamera : public Camera {
  public:
    const Frustum &getFrustum();
};

NS_CC_END
