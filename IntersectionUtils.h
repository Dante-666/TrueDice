#include "2d/CCCamera.h"
#include "3d/CCPlane.h"
#include "3d/CCRay.h"
#include "base/CCDirector.h"

#include "iostream"
#include <vector>

NS_CC_BEGIN

std::vector<Vec3> obtainIntersectionPoints(const Camera *cam);

const Ray intersection(const Plane &p1, const Plane &p2, const Vec3 &origin);
const Vec3 intersection(const Ray &r, const Plane &p);

NS_CC_END
