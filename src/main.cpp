#include <iostream>
#include <cstdio>

#include <object.hpp>
#include <vec3.hpp>
#include <triangle.hpp>
#include <bvh_node.hpp>
#include <bvh.hpp>

using namespace bvh;

void save_bvh_test_1()
{
  // Create test triangles for the BVH leaf nodes
  int num_triangles = 3;
  Triangle *test_triangles = new Triangle[num_triangles];

  test_triangles[0].vertices[0] = {0, 0, 0};
  test_triangles[0].vertices[1] = {1, 0, 0};
  test_triangles[0].vertices[2] = {0, 1, 0};

  test_triangles[1].vertices[0] = {0, 0, 0};
  test_triangles[1].vertices[1] = {1, 1, 0};
  test_triangles[1].vertices[2] = {0, 1, 0};

  test_triangles[2].vertices[0] = {0, 0, 1};
  test_triangles[2].vertices[1] = {1, 0, 1};
  test_triangles[2].vertices[2] = {0, 1, 1};

  // Create bounding boxes' minimums and maximums for the leaf nodes
  vec3<float> min_left_leaf_1(0, 0, 0);
  vec3<float> max_left_leaf_1(1, 1, 1);
  vec3<float> min_right_leaf_1(0, 0, 0);
  vec3<float> max_right_leaf_1(2, 2, 2);

  // Create leaf nodes
  BvhLeaf *left_leaf_1 = new BvhLeaf(min_left_leaf_1, max_left_leaf_1, 3, new int[3]{0, 1, 2});
  BvhLeaf *right_leaf_1 = new BvhLeaf(min_right_leaf_1, max_right_leaf_1, 3, new int[3]{0, 1, 3});

  // Create the root node
  vec3<float> root_min(0, 0, 0);
  vec3<float> root_max(2, 2, 2);
  BvhNode *root = new BvhNode(root_min, root_max);
  root->left = left_leaf_1;
  root->right = right_leaf_1;

  // Create an object to save the BVH
  vec3<float> position(0, 0, 0);
  vec3<float> rotation(0, 0, 0);
  vec3<float> scale(1, 1, 1);

  Object obj(position, rotation, scale, test_triangles, 3, root);

  // Save the BVH to a file
  const char *bvh_filename = "test_bvh.bvh";
  obj.save_bvh(const_cast<char *>(bvh_filename), root);
}

/*
         R
      /     \
    li1     ri1
   /  \     /  \
ll1   rl1 ll2  rl2
*/
void save_bvh_test_2()
{
  // Create test triangles for the BVH leaf nodes
  int num_triangles = 6;
  Triangle *test_triangles = new Triangle[num_triangles];

  test_triangles[0].vertices[0] = {0, 0, 0};
  test_triangles[0].vertices[1] = {1, 0, 0};
  test_triangles[0].vertices[2] = {0, 1, 0};

  test_triangles[1].vertices[0] = {0, 0, 0};
  test_triangles[1].vertices[1] = {1, 1, 0};
  test_triangles[1].vertices[2] = {0, 1, 0};

  test_triangles[2].vertices[0] = {0, 0, 1};
  test_triangles[2].vertices[1] = {1, 0, 1};
  test_triangles[2].vertices[2] = {0, 1, 1};

  // Create bounding boxes' minimums and maximums for the leaf nodes
  vec3<float> min_left_leaf_1(0, 0, 0);
  vec3<float> max_left_leaf_1(1, 1, 1);
  vec3<float> min_right_leaf_1(0, 0, 0);
  vec3<float> max_right_leaf_1(2, 2, 2);
  vec3<float> min_left_leaf_2(1, 2, 3);
  vec3<float> max_left_leaf_2(4, 3, 4);
  vec3<float> min_right_leaf_2(4, 3, 5);
  vec3<float> max_right_leaf_2(7, 5, 7);

  // Create leaf nodes
  BvhLeaf *left_leaf_1 = new BvhLeaf(min_left_leaf_1, max_left_leaf_1, 3, new int[3]{0, 1, 2});
  BvhLeaf *right_leaf_1 = new BvhLeaf(min_right_leaf_1, max_right_leaf_1, 3, new int[3]{0, 1, 3});
  BvhLeaf *left_leaf_2 = new BvhLeaf(min_left_leaf_2, max_left_leaf_2, 3, new int[3]{3, 2, 1});
  BvhLeaf *right_leaf_2 = new BvhLeaf(min_right_leaf_2, max_right_leaf_2, 3, new int[3]{2, 1, 0});

  // Create bounding boxes' minimums and maximums for the internal nodes
  vec3<float> min_left_internal_1(10, 10, 10);
  vec3<float> max_left_internal_1(11, 11, 11);
  vec3<float> min_right_internal_1(11, 12, 13);
  vec3<float> max_right_internal_1(14, 13, 14);

  // Create internal nodes
  BvhNode *left_internal_1 = new BvhNode(min_left_internal_1, max_left_internal_1);
  BvhNode *right_internal_1 = new BvhNode(min_right_internal_1, max_right_internal_1);
  left_internal_1->left = left_leaf_1;
  left_internal_1->right = right_leaf_1;
  right_internal_1->left = left_leaf_2;
  right_internal_1->right = right_leaf_2;

  // Create the root node
  vec3<float> root_min(0, 0, 0);
  vec3<float> root_max(2, 2, 2);
  BvhNode *root = new BvhNode(root_min, root_max);
  root->left = left_internal_1;
  root->right = right_internal_1;

  // Create an object to save the BVH
  vec3<float> position(0, 0, 0);
  vec3<float> rotation(0, 0, 0);
  vec3<float> scale(1, 1, 1);

  Object obj(position, rotation, scale, test_triangles, 3, root);

  // Save the BVH to a file
  const char *bvh_filename = "test_bvh.bvh";
  obj.save_bvh(const_cast<char *>(bvh_filename), root);
}

int main()
{
  // char obj_filename[] = "./test.obj";
  // char bvh_filename[] = "./test.bvh";

  // Object::build_bvh(obj_filename, bvh_filename);

  // save_bvh_test_1();
  save_bvh_test_2();

  return 0;
}
