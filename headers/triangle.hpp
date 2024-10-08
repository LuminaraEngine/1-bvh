#pragma once

#include <vec2.hpp>
#include <vec3.hpp>
#include <vector> // Include vector for std::vector

namespace bvh {

  typedef struct {
    vec3<float> vertices[3];    // Vertices coordinates
    vec2<float> uv[3];          // Texture coordinates
    vec3<float> normals[3];     // Normal vectors
    bool smooth;                // Smooth shading
  } Triangle;

  std::vector<Triangle> generateRandomTriangles(int numTriangles, float rangeMin, float rangeMax);
}
