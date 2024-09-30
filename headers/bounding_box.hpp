#pragma once

#include <float3.hpp>

namespace bvh {

  class BoundingBox {
  public:
    float3 min, max;  // vertices

    BoundingBox(float3 min, float3 max);
  };

}
