#include <float3.hpp>
#include <triangle.hpp>
#include <object.hpp>

#include <iostream>

int main() {
  bvh::float3 position = {0.0f, 0.0f, 0.0f};
  bvh::float3 rotation = {0.0f, 0.0f, 0.0f};
  bvh::float3 scale = {1.0f, 1.0f, 1.0f};

  bvh::Triangle triangle = {
    {0.0f, 0.0f, 0.0f},
    {1.0f, 0.0f, 0.0f},
    {0.0f, 1.0f, 0.0f}
  };

  bvh::Object object(position, rotation, scale, 1, &triangle);

  std::cout << "Object position: (" << object.position.x << ", " << object.position.y << ", " << object.position.z << ")" << std::endl;

  return 0;
}
