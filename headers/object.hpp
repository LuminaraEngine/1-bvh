#pragma once

#include <float3.hpp>
#include <triangle.hpp>
#include <bvh_node.hpp>

namespace bvh {

  class Object {
  public:
    float3 position;
    float3 rotation;
    float3 scale;

    Triangle* triangles;
    int num_triangles;

    BvhNode *bvh;

    static Object* build(char* obj_filename);

    static Object* build(char* obj_filename, char* bvh_filename);

    ~Object();

  private:
    Object(float3 position, float3 rotation, float3 scale, Triangle *triangles, int num_triangles, BvhNode* bvh);
  };

}
