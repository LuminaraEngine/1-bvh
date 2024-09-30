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

    static Object* build(char* obj_filename);

    static Object* build(char* obj_filename, char* bvh_filename);

    ~Object();

  private:
    Object(vec3<float> position, vec3<float> rotation, vec3<float> scale, Triangle *triangles, int num_triangles, BvhNode* bvh);
  };

}
