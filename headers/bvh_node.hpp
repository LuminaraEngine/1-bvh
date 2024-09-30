#pragma once

#include <vec3.hpp>
#include <triangle.hpp>
#include <bounding_box.hpp>

#define BVH_LEAF_SIZE 8

namespace bvh {

  class BvhNode {
    public:
      BvhNode *left, *right;
      BoundingBox bounding_box;

      BvhNode(vec3<float> min, vec3<float> max);
      ~BvhNode();
  };

  class BvhLeaf : public BvhNode {
    public:
      int num_triangles;
      int indices[BVH_LEAF_SIZE];

      BvhLeaf(vec3<float> min, vec3<float> max, int num_triangles, int* indices);

      ~BvhLeaf();
  };

}
