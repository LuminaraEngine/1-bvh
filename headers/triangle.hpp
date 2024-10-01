#pragma once

#include <vec3.hpp>

namespace bvh {

  typedef struct {
    vec3<float> vertices[3];    // Vertices coordinates
    vec3<float> uv[3];          // Texture coordinates
    vec3<float> normals[3];     // Normal vectors
    bool smooth;            // Smooth shading
  } Triangle;

}
