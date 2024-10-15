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
      ~BvhNode();
      virtual void print(int depth=0);
  };

  class BvhLeaf : public BvhNode {
    public:
      int num_triangles;
      int indices[BVH_LEAF_SIZE];

      BvhLeaf();
      BvhLeaf(vec3<float> min, vec3<float> max, int num_triangles, int* indices);
      ~BvhLeaf();
      void print(int depth) override;
  };

}
