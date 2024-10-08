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
      BvhNode(vec3<float> min, vec3<float> max): left(nullptr), right(nullptr), bounding_box(min, max) {};


      virtual ~BvhNode() {
        delete this->left;
        delete this->right;
      }
      virtual void print(){
      };
  };

  class BvhLeaf : public BvhNode {
    public:
      int num_triangles;
      int indices[BVH_LEAF_SIZE];

      BvhLeaf() : num_triangles(0) {};
      BvhLeaf(vec3<float> min, vec3<float> max, int num_triangles, int* triangle_indices)
          : BvhNode(min, max), num_triangles(num_triangles) {
          std::copy(triangle_indices, triangle_indices + num_triangles, indices);
      }
      ~BvhLeaf();

      void print() override {
      }
  };

}
