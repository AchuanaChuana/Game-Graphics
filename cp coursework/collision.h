//#pragma once
//
//#include <windows.h>
//#include "mathLibrary.h"
//#include "camera.h"
//#include "Mesh.h"
//
//struct AABB
//{
//    Vec3 minimum;
//    Vec3 maximum;
//
//    AABB() : minimum(FLT_MAX, FLT_MAX, FLT_MAX), maximum(-FLT_MAX, -FLT_MAX, -FLT_MAX) {}
//
//    void expand(const Vec3& point) 
//    {
//        if (point.x < minimum.x)  minimum.x = point.x;
//        if (point.y < minimum.y)  minimum.y = point.y;
//        if (point.z < minimum.z)  minimum.z = point.z;
//
//        if (point.x > maximum.x) maximum.x = point.x;
//        if (point.y > maximum.y) maximum.y = point.y;
//        if (point.z > maximum.z) maximum.z = point.z;
//    }
//};
//
//class collision
//{
//public:
//   static AABB transformAABB(const AABB& box, const Matrix44& world)
//{
//    std::vector<Vec3> corners = 
//    {
//        { box.minimum.x, box.minimum.y, box.minimum.z },
//        { box.minimum.x, box.minimum.y, box.maximum.z },
//        { box.minimum.x, box.maximum.y, box.minimum.z },
//        { box.minimum.x, box.maximum.y, box.maximum.z },
//        { box.maximum.x, box.minimum.y, box.minimum.z },
//        { box.maximum.x, box.minimum.y, box.maximum.z },
//        { box.maximum.x, box.maximum.y, box.minimum.z },
//        { box.maximum.x, box.maximum.y, box.maximum.z }
//    };
//
//    AABB transformed;
//    for (auto& c : corners)
//    {
//        Vec4 p(c.x, c.y, c.z, 1.0f);
//        Vec4 tp = world * p;
//        transformed.expand(Vec3(tp.x, tp.y, tp.z));
//    }
//    return transformed;
//}
//
//   bool AABBIntersect(const AABB& a, const AABB& b)
//  {
//      if (a.maximum.x < b.minimum.x || a.minimum.x > b.maximum.x) return false;
//      if (a.maximum.y < b.minimum.y || a.minimum.y > b.maximum.y) return false;
//      if (a.maximum.z < b.minimum.z || a.minimum.z > b.maximum.z) return false;
//      return true;
//  }
//
//   static bool checkCollision(Vec3 mini, Vec3 maxi,const FPCamera& camera, const Box& box, const Matrix44& boxWorldTransform)
//   {
//       AABB cameraAABB = camera.getCameraAABB();
//       AABB boxAABB = box.getAABB(boxWorldTransform, mini, maxi);
//       return collision::AABBIntersect(cameraAABB, boxAABB);
//   }
//
//}; 

