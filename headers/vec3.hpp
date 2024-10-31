#pragma once

#include <cmath>
#include <algorithm>
#include <iostream>

namespace bvh {

  template <typename T>
  class vec3 {
  public:
    T x, y, z;

    vec3() : x(0), y(0), z(0) {};
    vec3(T x, T y, T z) : x(x), y(y), z(z) {};

    vec3<T> operator+(T scalar) const {
        return vec3<T>(this->x + scalar, this->y + scalar, this->z + scalar);
    }

    vec3<T> operator+(const vec3<T>& other) const {
        return vec3<T>(this->x + other.x, this->y + other.y, this->z + other.z);
    }

    vec3<T> operator*(T scalar) const {
        return vec3<T>(this->x * scalar, this->y * scalar, this->z * scalar);
    }

    // Define the division operator by a scalar
    vec3<T> operator/(T scalar) const {
        return vec3<T>(this->x / scalar, this->y / scalar, this->z / scalar);
    }

    // Overloading the subtraction operator
    vec3<T> operator-(const vec3<T>& other) const {
        return vec3<T>(this->x - other.x, this->y - other.y, this->z - other.z);
    }

    bool operator==(const vec3<T>& other) const {
            return (x == other.x) && (y == other.y) && (z == other.z);
    }

    bool operator!=(const vec3<T>& other) const { // keep this below the operator== definition
        return !(*this == other);
    }

    friend std::ostream& operator<<(std::ostream& os, const vec3<T>& vec) {
        os << "vec3(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
        return os;
    }

    // Static method to get the minimum of two vec3
    static vec3<T> min(const vec3<T>& a, const vec3<T>& b) {
        return vec3<T>(
            std::min(a.x, b.x),
            std::min(a.y, b.y),
            std::min(a.z, b.z)
        );
    }

    // Static method to get the maximum of two vec3
    static vec3<T> max(const vec3<T>& a, const vec3<T>& b) {
        return vec3<T>(
            std::max(a.x, b.x),
            std::max(a.y, b.y),
            std::max(a.z, b.z)
        );
    }

    // Static method to get the dot product of two vec3
    static T dot(const vec3<T>& a, const vec3<T>& b) {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    // Static method to get the cross product of two vec3
    static vec3<T> cross(const vec3<T>& a, const vec3<T>& b) {
        return vec3<T>(
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x
        );
    }

    // Static method to normalize a vec3
    static vec3<T> normalize(const vec3<T>& a) {
        T length = sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
        return vec3<T>(a.x / length, a.y / length, a.z / length);
    }
  };

}
