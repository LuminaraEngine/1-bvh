#pragma once

#include <vec3.hpp>

namespace bvh {

  class BoundingBox {
  public:
    vec3<float> min, max;  // vertices

    BoundingBox(vec3<float> min, vec3<float> max);
  };

}
