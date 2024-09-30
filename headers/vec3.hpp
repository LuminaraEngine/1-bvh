#pragma once

namespace bvh {

  template <typename T>
  class vec3 {
  public:
    T x, y, z;

    vec3() : x(0), y(0), z(0) {};
    vec3(T x, T y, T z) : x(x), y(y), z(z) {};
  };

}
