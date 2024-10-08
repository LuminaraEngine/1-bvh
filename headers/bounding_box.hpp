#pragma once

#include <vec3.hpp>

namespace bvh {

  class BoundingBox {
  public:
    vec3<float> min, max;  // vertices

    BoundingBox() : min(vec3<float>(0.0f, 0.0f, 0.0f)), max(vec3<float>(0.0f, 0.0f, 0.0f)) {}

    BoundingBox(vec3<float> min, vec3<float> max);


  };


}
