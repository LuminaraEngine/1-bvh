#include <bvh_node.hpp>

#include <cstdio>

using namespace bvh;

BvhNode::BvhNode() : bounding_box(vec3<float>(0, 0, 0), vec3<float>(0, 0, 0)) {
  this->left = nullptr;
  this->right = nullptr;
}

BvhNode::BvhNode(vec3<float> min, vec3<float> max) : bounding_box(min, max) {
  this->left = nullptr;
  this->right = nullptr;
}

BvhNode::~BvhNode() {
  if (left) {
    delete left;
    left = nullptr;  // Set to nullptr after deletion to avoid dangling pointer issues
  }
  if (right) {
    delete right;
    right = nullptr;  // Same here
  }
}

void BvhNode::print() {
  printf("BvhNode\n");
  printf("min: %f %f %f\n", this->bounding_box.min.x, this->bounding_box.min.y, this->bounding_box.min.z);
  printf("max: %f %f %f\n", this->bounding_box.max.x, this->bounding_box.max.y, this->bounding_box.max.z);
  this->left->print();
  this->right->print();
}

BvhLeaf::BvhLeaf() : BvhNode() {
  this->num_triangles = 0;
}

BvhLeaf::BvhLeaf(vec3<float> min, vec3<float> max, int num_triangles, int* indices) : BvhNode(min, max) {
  this->num_triangles = num_triangles;
  for (int i = 0; i < num_triangles; i++) {
    this->indices[i] = indices[i];
  }
}

BvhLeaf::~BvhLeaf() {}

void BvhLeaf::print() {
  printf("BvhLeaf\n");
  printf("min: %f %f %f\n", this->bounding_box.min.x, this->bounding_box.min.y, this->bounding_box.min.z);
  printf("max: %f %f %f\n", this->bounding_box.max.x, this->bounding_box.max.y, this->bounding_box.max.z);
  printf("num_triangles: %d\n", this->num_triangles);
  for (int i = 0; i < this->num_triangles; i++) {
    printf("%i ", this->indices[i]);
  }
  printf("\n");
}
