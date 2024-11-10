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

      BvhNode();
      BvhNode(vec3<float> min, vec3<float> max);
      virtual ~BvhNode();
      virtual void print(int depth=0);

      // Method to create a separate copy of the BVH tree rooted at the current node
      virtual BvhNode* clone() const;
  };

  class BvhLeaf : public BvhNode { // BvhLeaf inherits from BvhNode (special type of BvhNode)
    public:
      int num_triangles;
      int indices[BVH_LEAF_SIZE];

      BvhLeaf();
      BvhLeaf(vec3<float> min, vec3<float> max, int num_triangles, int* indices);
      ~BvhLeaf();
      void print(int depth) override;

      // Method to create a separate copy of the leaf node
      BvhNode* clone() const override;
  };

}
