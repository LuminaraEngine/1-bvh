#include <bvh_node.hpp>
#include <iostream>

using namespace bvh;

BvhNode::BvhNode(vec3<float> min, vec3<float> max) : bounding_box(min, max) {
  this->left = nullptr;
  this->right = nullptr;
}

BvhNode::~BvhNode() {
    if (this->left) {
        std::cout << "Deleting left child at " << this->left << std::endl;
        delete this->left; // Delete left child if it exists
        this->left = nullptr; // Set to nullptr to avoid double deletion
    }
    if (this->right) {
        std::cout << "Deleting right child at " << this->right << std::endl;
        delete this->right; // Delete right child if it exists
        this->right = nullptr; // Set to nullptr to avoid double deletion
    }
}

BvhLeaf::BvhLeaf(vec3<float> min, vec3<float> max, int num_triangles, int* indices) : BvhNode(min, max) {
  this->num_triangles = num_triangles;
  for (int i = 0; i < num_triangles; i++) {
    this->indices[i] = indices[i];
  }
}

BvhLeaf::~BvhLeaf() {}
