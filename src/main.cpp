#include <vec3.hpp>
#include <triangle.hpp>
#include <bvh_node.hpp>

#include <iostream>

int main() {
  bvh::vec3<float> a;

  bvh::vec3<float> position(0.0f, 0.0f, 0.0f);
  bvh::vec3<float> rotation(0.0f, 0.0f, 0.0f);
  bvh::vec3<float> scale(1.0f, 1.0f, 1.0f);

  bvh::Triangle triangle = {
    bvh::vec3<float>(0.0f, 0.0f, 0.0f),
    bvh::vec3<float>(1.0f, 0.0f, 0.0f),
    bvh::vec3<float>(0.0f, 1.0f, 0.0f),
    bvh::vec3<float>(0.0f, 0.0f, 1.0f)
  };

  bvh::BvhNode bvh_root = bvh::BvhNode({-1.0f, -1.0f, -1.0f}, {1.0f, 1.0f, 1.0f});
  bvh_root.left = new bvh::BvhLeaf({-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 0.0f}, 1, new int[1]{0});
  bvh_root.right = new bvh::BvhLeaf({0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, 1, new int[1]{1});

  return 0;
}
