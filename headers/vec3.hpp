#pragma once

namespace bvh {

  template <typename T>
  class vec3 {
  public:
    T x, y, z;

    vec3() : x(0), y(0), z(0) {};
    vec3(T x, T y, T z) : x(x), y(y), z(z) {};

    // Overload the + operator
    vec3 operator+(const vec3& other) const {
        return vec3(x + other.x, y + other.y, z + other.z);
    }

        // Overload the / operator for scalar division
    vec3 operator/(T scalar) const {
        return vec3(x / scalar, y / scalar, z / scalar);
    }
  };

}
