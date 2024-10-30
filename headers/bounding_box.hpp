#pragma once

#include <vec3.hpp>

namespace bvh {

  class BoundingBox {
  public:
    vec3<float> min, max;  // vertices

    BoundingBox() : min(vec3<float>(0.0f, 0.0f, 0.0f)), max(vec3<float>(0.0f, 0.0f, 0.0f)) {}

    BoundingBox(vec3<float> min, vec3<float> max);

    bool operator==(const BoundingBox& other) const;

    float surfaceArea() const {
        vec3<float> size = { max.x - min.x, max.y - min.y, max.z - min.z };
        return 2.0f * (size.x * size.y + size.x * size.z + size.y * size.z);
    }

    void expand(const BoundingBox& other) {
        min.x = std::min(min.x, other.min.x);
        min.y = std::min(min.y, other.min.y);
        min.z = std::min(min.z, other.min.z);
        max.x = std::max(max.x, other.max.x);
        max.y = std::max(max.y, other.max.y);
        max.z = std::max(max.z, other.max.z);
    }


  };


}
