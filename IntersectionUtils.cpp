#include "IntersectionUtils.h"

NS_CC_BEGIN

std::vector<Vec3> obtainIntersectionPoints(const Camera *cam) {

    const Mat4 &mat = cam->getViewProjectionMatrix();
    // ref
    // http://www.lighthouse3d.com/tutorials/view-frustum-culling/clip-space-approach-extracting-the-planes/
    // extract frustum plane
    Plane _plane[6];
    _plane[0].initPlane(
        -Vec3(mat.m[3] + mat.m[0], mat.m[7] + mat.m[4], mat.m[11] + mat.m[8]),
        (mat.m[15] + mat.m[12])); // left
    _plane[1].initPlane(
        -Vec3(mat.m[3] - mat.m[0], mat.m[7] - mat.m[4], mat.m[11] - mat.m[8]),
        (mat.m[15] - mat.m[12])); // right
    _plane[2].initPlane(
        -Vec3(mat.m[3] + mat.m[1], mat.m[7] + mat.m[5], mat.m[11] + mat.m[9]),
        (mat.m[15] + mat.m[13])); // bottom
    _plane[3].initPlane(
        -Vec3(mat.m[3] - mat.m[1], mat.m[7] - mat.m[5], mat.m[11] - mat.m[9]),
        (mat.m[15] - mat.m[13])); // top
    /*	Don't need the near and far, instead, come up with the ones
        we are using. Since the coordinates are in world space, we can use these
        equations
    */
    _plane[4].initPlane(Vec3(-1, 0, 0), 1.0);
    _plane[5].initPlane(Vec3(1, 0, 0), 1.0);

    /*	Intersect left with top and bottom
        and do right with top and bottom to get 4 lines.
    */
    Vec3 origin = cam->getPosition3D();

    Ray _rays[4];
    _rays[0] = intersection(_plane[0], _plane[3], origin);
    _rays[1] = intersection(_plane[0], _plane[2], origin);
    _rays[2] = intersection(_plane[1], _plane[3], origin);
    _rays[3] = intersection(_plane[1], _plane[2], origin);

#ifdef COCOS2D_DEBUG
    auto viewRays = [&_rays]() {
        for (int i = 0; i < 4; i++) {
            std::cout << "Origin: (" << _rays[i]._origin.x << ", "
                      << _rays[i]._origin.y << ", " << _rays[i]._origin.z << ")"
                      << std::endl;
            std::cout << "Direction: (" << _rays[i]._direction.x << ", "
                      << _rays[i]._direction.y << ", " << _rays[i]._direction.z
                      << ")" << std::endl;
        }
    };
    viewRays();
#endif

    /*	Then use ray-plane intersection to get the corners of collision quad
     */
    std::vector<Vec3> corners;
    Vec3 corner;
    auto intersectionHelper = [&corner, &corners, &_rays,
                               &_plane](int rayIdx, int planeIdx) {
        corner = intersection(_rays[rayIdx], _plane[planeIdx]);
        corners.push_back(corner);
    };
    intersectionHelper(0, 4);
    intersectionHelper(1, 4);
    intersectionHelper(2, 4);
    intersectionHelper(3, 4);
    intersectionHelper(0, 5);
    intersectionHelper(1, 5);
    intersectionHelper(2, 5);
    intersectionHelper(3, 5);

    return corners;
};

const Ray intersection(const Plane &p1, const Plane &p2, const Vec3 &origin) {
    /*	Two planes intersect at a line and the line direction
        is given by the corss product between the plane normals
    */
    Vec3 dir;
    Vec3::cross(p1.getNormal(), p2.getNormal(), &dir);

    // We could have parallel checks but we know these planes intersect

    /*	We know the planes would intersect at the camera origin

        Line = p + t * dir
    */
    Ray r;
    r.set(origin, dir);

    return r;
};

const Vec3 intersection(const Ray &r, const Plane &p) {

    // a*Vx + b*Vy + c*Vz
    float dot1 = p.getNormal().dot(r._direction);
    // a*x1 + b*y1 + c*z1
    float dot2 = p.getNormal().dot(r._origin);

    // If denominator=0, no intersect
    // However, we can avoid these checks

    // find t = -(a*x1 + b*y1 + c*z1 + d) / (a*Vx + b*Vy + c*Vz)
    float t = -(dot2 + p.getDist()) / dot1;

    // find intersection point
    return r._origin + (t * r._direction);
};

NS_CC_END
