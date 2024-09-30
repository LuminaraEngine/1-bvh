#include <bvh_node.hpp>

using namespace bvh;

BvhNode::BvhNode(BvhNode* left, BvhNode* right) {
  this->left = left;
  this->right = right;
}

BvhNode::~BvhNode() {
  delete left;
  delete right;
}

BvhLeaf::BvhLeaf(int num_triangles, int* indices) : BvhNode(nullptr, nullptr) {
  this->num_triangles = num_triangles;
  for (int i = 0; i < num_triangles; i++) {
    this->indices[i] = indices[i];
  }
}

BvhLeaf::~BvhLeaf() {
  delete[] indices;
}
