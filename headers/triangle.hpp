#pragma once

#include <vec3.hpp>

namespace bvh {

  typedef struct {
    vec3<float> a, b, c;   // Vertices
    vec3<float> normal;    // Normal vector
  } Triangle;

}
