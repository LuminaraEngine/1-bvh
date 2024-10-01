#pragma once

namespace bvh {

  template <typename T>
  class vec2 {
  public:
    T x, y;

    vec2() : x(0), y(0) {};
    vec2(T x, T y) : x(x), y(y) {};
  };

}
