#pragma once

#include <vec3.hpp>
#include <triangle.hpp>
#include <bvh_node.hpp>

namespace bvh {

  class Object {
  public:
    vec3<float> position;
    vec3<float> rotation;
    vec3<float> scale;

    Triangle* triangles;
    int num_triangles;

    BvhNode *bvh;

    static void build_bvh(char* obj_filename, char* bvh_filename);

    static Object* load(char* obj_filename, char* bvh_filename);

    ~Object();

  private:
    Object(vec3<float> position, vec3<float> rotation, vec3<float> scale, Triangle *triangles, int num_triangles, BvhNode* bvh);
  };

}
