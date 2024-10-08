#include <iostream>
#include <cstdio>

#include <test_save_bvh.hpp>
#include <object.hpp>
#include <vec3.hpp>
#include <triangle.hpp>
#include <bvh_node.hpp>
#include <bvh.hpp>

void bvh::tests::save_bvh_test_1()
{
    // Create test triangles for the BVH leaf nodes
    int num_triangles = 3;
    bvh::Triangle *test_triangles = new bvh::Triangle[num_triangles];

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
    bvh::vec3<float> min_left_leaf_1(0, 0, 0);
    bvh::vec3<float> max_left_leaf_1(1, 1, 1);
    bvh::vec3<float> min_right_leaf_1(0, 0, 0);
    bvh::vec3<float> max_right_leaf_1(2, 2, 2);

    // Create leaf nodes
    bvh::BvhLeaf *left_leaf_1 = new bvh::BvhLeaf(min_left_leaf_1, max_left_leaf_1, 3, new int[3]{0, 1, 2});
    bvh::BvhLeaf *right_leaf_1 = new bvh::BvhLeaf(min_right_leaf_1, max_right_leaf_1, 3, new int[3]{0, 1, 3});

    // Create the root node
    bvh::vec3<float> root_min(0, 0, 0);
    bvh::vec3<float> root_max(2, 2, 2);
    bvh::BvhNode *root = new bvh::BvhNode(root_min, root_max);
    root->left = left_leaf_1;
    root->right = right_leaf_1;

    // Create an object to save the BVH
    bvh::vec3<float> position(0, 0, 0);
    bvh::vec3<float> rotation(0, 0, 0);
    bvh::vec3<float> scale(1, 1, 1);

    bvh::Object obj(position, rotation, scale, test_triangles, 3, root);

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
    bvh::Triangle *test_triangles = new bvh::Triangle[num_triangles];

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
    bvh::vec3<float> min_left_leaf_1(0, 0, 0);
    bvh::vec3<float> max_left_leaf_1(1, 1, 1);
    bvh::vec3<float> min_right_leaf_1(0, 0, 0);
    bvh::vec3<float> max_right_leaf_1(2, 2, 2);
    bvh::vec3<float> min_left_leaf_2(1, 2, 3);
    bvh::vec3<float> max_left_leaf_2(4, 3, 4);
    bvh::vec3<float> min_right_leaf_2(4, 3, 5);
    bvh::vec3<float> max_right_leaf_2(7, 5, 7);

    // Create leaf nodes
    bvh::BvhLeaf *left_leaf_1 = new bvh::BvhLeaf(min_left_leaf_1, max_left_leaf_1, 3, new int[3]{0, 1, 2});
    bvh::BvhLeaf *right_leaf_1 = new bvh::BvhLeaf(min_right_leaf_1, max_right_leaf_1, 3, new int[3]{0, 1, 3});
    bvh::BvhLeaf *left_leaf_2 = new bvh::BvhLeaf(min_left_leaf_2, max_left_leaf_2, 3, new int[3]{3, 2, 1});
    bvh::BvhLeaf *right_leaf_2 = new bvh::BvhLeaf(min_right_leaf_2, max_right_leaf_2, 3, new int[3]{2, 1, 0});

    // Create bounding boxes' minimums and maximums for the internal nodes
    bvh::vec3<float> min_left_internal_1(10, 10, 10);
    bvh::vec3<float> max_left_internal_1(11, 11, 11);
    bvh::vec3<float> min_right_internal_1(11, 12, 13);
    bvh::vec3<float> max_right_internal_1(14, 13, 14);

    // Create internal nodes
    bvh::BvhNode *left_internal_1 = new bvh::BvhNode(min_left_internal_1, max_left_internal_1);
    bvh::BvhNode *right_internal_1 = new bvh::BvhNode(min_right_internal_1, max_right_internal_1);
    left_internal_1->left = left_leaf_1;
    left_internal_1->right = right_leaf_1;
    right_internal_1->left = left_leaf_2;
    right_internal_1->right = right_leaf_2;

    // Create the root node
    bvh::vec3<float> root_min(0, 0, 0);
    bvh::vec3<float> root_max(2, 2, 2);
    bvh::BvhNode *root = new bvh::BvhNode(root_min, root_max);
    root->left = left_internal_1;
    root->right = right_internal_1;

    // Create an object to save the BVH
    bvh::vec3<float> position(0, 0, 0);
    bvh::vec3<float> rotation(0, 0, 0);
    bvh::vec3<float> scale(1, 1, 1);

    bvh::Object obj(position, rotation, scale, test_triangles, 3, root);

    // Save the BVH to a file
    const char *bvh_filename = "test_bvh.bvh";
    obj.save_bvh(const_cast<char *>(bvh_filename), root);
}
