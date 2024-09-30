#pragma once

#include <triangle.hpp>
#include <bounding_box.hpp>

#define BVH_LEAF_SIZE 8

namespace bvh {

  class BvhNode {
    public:
      BvhNode *left, *right;
      BoundingBox bounding_box;

      BvhNode(BvhNode* left, BvhNode* right);
      ~BvhNode();
  };

  class BvhLeaf : public BvhNode {
    public:
      int num_triangles;
      int indices[BVH_LEAF_SIZE];

      BvhLeaf(int num_triangles, int* indices);

      ~BvhLeaf();
  };

}
