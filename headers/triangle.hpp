#pragma once

#include <float3.hpp>

namespace bvh {

  typedef struct {
    float3 a, b, c;   // Vertices
    float3 normal;    // Normal vector
  } Triangle;

}
