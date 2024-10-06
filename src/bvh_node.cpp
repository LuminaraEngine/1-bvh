#include <bvh_node.hpp>

using namespace bvh;

BvhNode::BvhNode(vec3<float> min, vec3<float> max) : bounding_box(min, max)
{
  this->left = nullptr;
  this->right = nullptr;
}

BvhNode::~BvhNode()
{
  delete this->left;
  delete this->right;
}

BvhLeaf::BvhLeaf(vec3<float> min, vec3<float> max, int num_triangles, int *indices) : BvhNode(min, max)
{
  this->num_triangles = num_triangles;
  for (int i = 0; i < num_triangles; i++)
  {
    this->indices[i] = indices[i];
  }
}

BvhLeaf::~BvhLeaf() {}
