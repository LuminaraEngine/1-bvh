#pragma once

#include <float3.hpp>
#include <triangle.hpp>

namespace bvh {

  class Object {
  public:
    float3 position;
    float3 rotation;
    float3 scale;

    int num_triangles;
    Triangle* triangles;

    Object(float3 position, float3 rotation, float3 scale, int num_triangles, Triangle* triangles) {
      this->position = position;
      this->rotation = rotation;
      this->scale = scale;
      this->num_triangles = num_triangles;
      this->triangles = triangles;
    }

    ~Object() {
      delete[] triangles;
    }
  };

}
