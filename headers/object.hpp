#pragma once

#include <vec3.hpp>
#include <triangle.hpp>
#include <bvh_node.hpp>

namespace bvh
{

  class Object
  {
  public:
    vec3<float> position; // Vector specifying where the object is placed in space
    vec3<float> rotation; // Vector specifying the prientation of the object
    vec3<float> scale;    // Vector specifying the cale of the object in the 3D world

    Triangle *triangles; // Triangles stored to represent the surface of the object
    int num_triangles;   // Number of triangles in the object

    BvhNode *bvh; // Pointer to the root node of the object's BVH

    static void build_bvh(char *obj_filename, char *bvh_filename);

    static void save_bvh(char *bvh_filename, BvhNode *bvh);

    static Object *load(char *obj_filename, char *bvh_filename);

    // Method to get the BVH
    BvhNode* getBvh() const {
        return bvh;
    }
    
    ~Object();
    
  public:
    Object(vec3<float> position, vec3<float> rotation, vec3<float> scale, Triangle *triangles, int num_triangles, BvhNode *bvh);
  };

  // private:
  //   Object(vec3<float> position, vec3<float> rotation, vec3<float> scale, Triangle *triangles, int num_triangles, BvhNode* bvh);
  // };
}
